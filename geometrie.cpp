#include "geometrie.h"


bool Sommet::operator ==(Sommet & comp){
    if(this == &comp) return true;
    return false;
}


bool Delaunay::isOutCircle(Point p1, Point p2, Point p3, Point np){
    double mat[10][10];
    mat[0][0] = p2.x - p3.x;
    mat[0][1] = p1.x - p3.x;
    mat[0][2] = np.x - p3.x;
    mat[1][0] = p2.y - p3.y;
    mat[1][1] = p1.y - p3.y;
    mat[1][2] = np.y - p3.y;
    mat[2][0] = (p2.x - p3.x)*(p2.x - p3.x) + (p2.y - p3.y)*(p2.y - p3.y);
    mat[2][1] = (p1.x - p3.x)*(p1.x - p3.x) + (p1.y - p3.y)*(p1.y - p3.y);
    mat[2][2] = (np.x - p3.x)*(np.x - p3.x) + (np.y - p3.y)*(np.y - p3.y);

    double det = 0.0;
    det = -deter(3, mat, det);
    std::cout << det << std::endl;
    if(det > 0) return true;
    return false;
}

double Delaunay::deter(int n, double mat[10][10], double & d){

    int c, subi, i, j, subj;

    double submat[10][10];

    if (n == 2){
        return( (mat[0][0] * mat[1][1]) - (mat[1][0] * mat[0][1]));

    }
    else{

        for(c = 0; c < n; c++){

            subi = 0;

            for(i = 1; i < n; i++){

                subj = 0;

                for(j = 0; j < n; j++){

                    if (j == c){
                        continue;
                    }
                    submat[subi][subj] = mat[i][j];
                    subj++;

                }
                subi++;

            }

        d = d + (pow(-1 ,c) * mat[0][c] * deter(n - 1 ,submat, d));

        }

    }

    return d;

}
