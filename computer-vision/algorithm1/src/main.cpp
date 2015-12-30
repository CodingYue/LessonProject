#include <opencv2/opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;

double square(double x)
{
    return x * x;
}

double diff(const Mat& src, int x1, int y1, int x2, int y2)
{
    double sum = 0;
    for (int t = 0; t < 3; ++t) {
        sum += square(src.at<Vec3b>(x1, y1)[t] - src.at<Vec3b>(x2, y2)[t]);
    }
    return sqrt(sum);
}
void normalize(vector<double> &mask)
{
    int len = mask.size();
    double sum = 0;
    for (int i = 1; i < len; ++i) {
        sum += fabs(mask[i]);
    }
    sum = sum*2 + fabs(mask[0]);
    for (int i = 0; i < len; ++i) {
        mask[i] /= sum;
    }
}

vector<double> make_fgauss(double sigma)
{
    sigma = max(sigma, 0.01);
    int len = (int) ceil(sigma * 4) + 1;
    vector<double> mask(len);
    for (int i = 0; i < len; ++i) {
        mask[i] = exp(-0.5*square(i/sigma));
    }
    return mask;
}

void convolve(Mat *src, Mat *dst, vector<double> mask)
{
    int len = mask.size();
    for (int t = 0; t < 3; ++t) {
        for (int y = 0; y < src->rows; ++y)
            for (int x = 0; x < src->cols; ++x) {
                double sum = mask[0] * src->at<Vec3b>(x, y)[t];
                for (int i = 1; i < len; ++i) {
                    sum += mask[i] * (src->at<Vec3b>(max(x-i, 0), y)[t] + src->at<Vec3b>(min(x+i, src->cols-1), y)[t]);
                }
                dst->at<Vec3b>(y, x) = sum;
            }
    }
}

Mat smooth(Mat src, double sigma)
{
    vector<double> mask = make_fgauss(sigma);
    normalize(mask);
    Mat *tmp = new Mat(src.cols, src.rows, 0);
    Mat *res = new Mat(src.rows, src.cols, 0);
    convolve(&src, tmp, mask);
    convolve(tmp, res, mask);
    return *res;
}

struct EDGE {
    int a, b;
    double w;
    EDGE(){}
    EDGE(int a, int b, int w) : a(a), b(b), w(w) {}
    bool operator < (const EDGE &b) const {
        return w < b.w;
    }
};

class universe
{
private:
    int *fa;
    int *size;
public:
    int find(int x)
    {
        if (x == fa[x]) return x;
        return fa[x] = find(fa[x]);
    }
    void join(int a, int b)
    {
        a = find(a);
        b = find(b);
        size[b] += a;
        this->fa[a] = b;
    }
    int get_size(int x)
    {
        return size[this->find(x)];
    }
    universe(int _size)
    {
        size = new int[_size];
        fa = new int[_size];
        for (int i = 0; i < _size; ++i) {
            fa[i] = i;
            size[i] = 1;
        }

    }
    ~universe() {
        delete [] this->fa;
        delete [] this->size;
    }
    
};
#define THRESHOLD(size, c) (c/size)

universe * segment_graph(int n, vector<EDGE> &edges, double c)
{
    sort(edges.begin(), edges.end());
    universe *ufs = new universe(n);

    double *threshold = new double[n];
    for (int i = 0; i < n; ++i) {
        threshold[i] = THRESHOLD(1, c);
    }
    for (auto it : edges) {
        int a = ufs->find(it.a);
        int b = ufs->find(it.b);
        if (a != b) {
            if (it.w <= threshold[a] && it.w <= threshold[b]) {
                ufs->join(a, b);
                a = ufs->find(a);
                threshold[a] = it.w + THRESHOLD(ufs->get_size(a), c);
            }
        }
    }
    //for (int i = 0; i < n; ++i) printf("%d ", ufs->find(i));
    return ufs;
}

int colors[11111111][3];

Mat segment_image(Mat src, double sigma, double k, int min_size)
{
    int rows = src.rows;
    int cols = src.cols;

    src = smooth(src, sigma);
    vector<EDGE> edges;
    for (int x = 0; x < src.rows; ++x)
        for (int y = 0; y < src.cols; ++y) {
            if (x+1 < src.rows) {
                edges.push_back(EDGE(x * src.cols + y, (x+1) * src.cols + y, diff(src, x, y, x+1, y)));
            }
            if (y+1 < src.cols) {
                edges.push_back(EDGE(x * src.cols + y, x * src.cols+y+1, diff(src, x, y, x, y+1)));
            }
            if (x+1 < src.rows && y+1 < src.cols) {
                edges.push_back(EDGE(x * src.cols + y, (x+1) * src.cols+y+1, diff(src, x, y, x+1, y+1)));
            }
            if (x+1 < src.rows && y > 0) {
                edges.push_back(EDGE(x * src.cols + y, (x+1) * src.cols+y-1, diff(src, x, y, x+1, y-1)));
            }
        }
    printf("%u\n", edges.size());
    universe *ufs = segment_graph(src.cols * src.rows, edges, k);
    for (auto it : edges) {
        int a = ufs->find(it.a);
        int b = ufs->find(it.b);
        if (a != b && (ufs->get_size(a) < min_size || ufs->get_size(b) < min_size)) {
            ufs->join(a, b);
        }
    }

    for (int i = 0; i < src.cols * src.rows; ++i) {
        for (int t = 0; t < 3; ++t) {
            colors[i][t] = random() % 256;
        }
    }
    Mat res = src;
    for (int x = 0; x < src.rows; ++x)
        for (int y = 0; y < src.cols; ++y) {
            int comp = ufs->find(x * src.cols + y);
            //printf("%d\n", comp);
            for (int t = 0; t < 3; ++t)
                res.at<Vec3b>(x, y)[t] = colors[comp][t];
        }
    return res;
}

int main(int argc, char** argv)
{
    if (argc != 6) {
        fprintf(stderr, "usage: %s sigma k min input(ppm) output(ppm)\n", argv[0]);
        return 1;
    }

    srand(time(0));

    double sigma = atof(argv[1]);
    double k = atof(argv[2]);
    int min_size = atoi(argv[3]);
    Mat src = imread(argv[4]);
    
    Mat res = segment_image(src, sigma, k, min_size);

    imshow("haha", res);
    waitKey(0);

    return 0;
}
