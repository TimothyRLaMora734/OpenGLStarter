
#ifndef BresenhamIterator_h
#define BresenhamIterator_h

#include <aribeiro/common.h>

namespace aRibeiro {

/// \brief Classical line iterator algorithm
///
/// Generate a raster of a line specified by two points.
/// \author Alessandro Ribeiro
///
class ARIBEIRO_API BresenhamIterator{
    int deltaCol ,
        deltaRow ,
        fraction,
        nextCol ,
        nextRow ,
        endCol ,
        endRow ;
    int stepRow, stepCol;
    int iterateCol;//if not, is to iterate ow
    public:
    /// \brief Initialize the Bresenham algorithm state
    ///
    /// It is needed to specify the source and target point as integers.
    ///
    /// There are several algorithms to rasterize lines, but the Bresenham
    /// is eficient and it dont uses floating points to compute the intermediate
    /// points to make the line.
    ///
    /// \author Alessandro Ribeiro
    /// \param x origin x
    /// \param y origin y
    /// \param xd target x
    /// \param yd target y
    ///
    BresenhamIterator(const int x,const int y,const int xd,const int yd);
    /// \brief Returns true until the algorithm reachs the target point
    /// \author Alessandro Ribeiro
    /// \return True if the target point is reached by the iteration
    ///
    bool next();
    /// \brief Reads the current point of the rasterization state
    /// \author Alessandro Ribeiro
    /// \param x Returns the current x of the iteration
    /// \param y Returns the current y of the iteration
    ///
    void getXY(int *x,int *y);

    /// \brief Returns true until the algorithm reachs the target point.
    /// Reads the current point of the rasterization state
    /// \author Alessandro Ribeiro
    /// \param x Returns the current x of the iteration
    /// \param y Returns the current y of the iteration
    /// \return True if the target point is reached by the iteration
    ///
    bool next(int *x,int *y);

};

}

#endif


/*

void bresenhamIt(bool matrix[][resx], int x,int y, int xd, int yd){
  BresenhamIterator it(x,y, xd, yd);
  it.getXY(&x,&y);
  matrix[y][x] = true;
  while(it.next(&x,&y)){
    matrix[y][x] = true;
  }

}


void bresenham(void (*putPixel)(const int x,const int y,void* p),
               void* p,
               int x,int y,
               int xd, int yd){

  int deltaCol = xd-x,
      deltaRow = yd-y,
      fraction,
      nextCol = x,
      nextRow = y,
      endCol = xd,
      endRow = yd;

  int stepRow, stepCol;


  if (deltaRow <  0){
    stepRow=-1;
    deltaRow = -deltaRow*2;
  }else
    stepRow=1;
  if (deltaCol <  0){
    stepCol=-1;
    deltaCol = -deltaCol*2;
  }else
    stepCol=1;

  putPixel(nextCol,nextRow,p);

  if (deltaCol >deltaRow){
    fraction = deltaRow*2-deltaCol;
    while (nextCol != endCol){
      if (fraction >=0){
        nextRow =nextRow +stepRow;
        fraction =fraction -deltaCol;
      }
      nextCol=nextCol+stepCol;
      fraction=fraction +deltaRow;

      putPixel(nextCol,nextRow,p);

    }
  }else{
    fraction = deltaCol*2-deltaRow;
    while (nextRow !=endRow){
      if (fraction >=0){
        nextCol=nextCol+stepCol;
        fraction=fraction -deltaRow;
      }
      nextRow =nextRow +stepRow;
      fraction=fraction +deltaCol;

      putPixel(nextCol,nextRow,p);

    }
  }
}




void putPixel(const int x,const int y, void* p){
  bool *m = (bool *)p;
  m[y*resx+ x] = true;
}


int main(){
  cleanM(matrix, resx, resy);
  //bresenhamIt(matrix,            2,3,            1,4);
  bresenham(putPixel,
            (void*)&matrix[0][0],
            2,3,
            1,4);
  printM(matrix, resx, resy);
  return 0;
}
*/
