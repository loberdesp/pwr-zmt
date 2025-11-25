#include "gsv-Cuboid.hh"
#include "gtsDebug.hh"
#include <iostream>
#include <sstream>
#include <QGLViewer/qglviewer.h>
#include <cmath>

using namespace std;


inline
unsigned short int MakeBrighter(unsigned short int Lum)
{
  #define SHIFT  20
  
  if (Lum > (255-SHIFT)) return 255;
  return Lum+SHIFT;
}





#define Vx(Sign) (Sign 0.5)
#define COORDS3(Vec)  Vec[0],Vec[1],Vec[2]
#define OPO_COORDS3(Vec)  -Vec[0],-Vec[1],-Vec[2]
#define REV_COORD(Val) (Val ? 1/Val:0)
#define REV_COORDS3(Vec)  REV_COORD(Vec[0]),REV_COORD(Vec[1]),REV_COORD(Vec[2])


void DrawCuboid()
{
  glBegin(GL_QUADS);
   glNormal3f(-1,0,0);
   glVertex3f( Vx(-), Vx(-),  Vx(-));  // Boczna x=-1
   glVertex3f( Vx(-), Vx(+),  Vx(-));
   glVertex3f( Vx(-), Vx(+),  Vx(+));
   glVertex3f( Vx(-), Vx(-),  Vx(+));

   
   glNormal3f(0,-1,0);
   glVertex3f( Vx(-), Vx(-),  Vx(-));  // Boczna y=-1
   glVertex3f( Vx(+), Vx(-),  Vx(-));
   glVertex3f( Vx(+), Vx(-),  Vx(+));
   glVertex3f( Vx(-), Vx(-),  Vx(+));

   glNormal3f(0,1,0);
   glVertex3f( Vx(-), Vx(+),  Vx(-));  // Boczna y=+1
   glVertex3f( Vx(+), Vx(+),  Vx(-));
   glVertex3f( Vx(+), Vx(+),  Vx(+));
   glVertex3f( Vx(-), Vx(+),  Vx(+));   


   glNormal3f(0,0,-1);
   glVertex3f( Vx(-), Vx(-),  Vx(-));  // Boczna z=-1
   glVertex3f( Vx(+), Vx(-),  Vx(-));
   glVertex3f( Vx(+), Vx(+),  Vx(-));
   glVertex3f( Vx(-), Vx(+),  Vx(-));

   glNormal3f(0,0,1);
   glVertex3f( Vx(-), Vx(-),  Vx(+));  // Boczna z=+1
   glVertex3f( Vx(+), Vx(-),  Vx(+));
   glVertex3f( Vx(+), Vx(+),  Vx(+));
   glVertex3f( Vx(-), Vx(+),  Vx(+));   
   /*
   glColor3ub(MakeBrighter(rColor.GetColor_Red()),
	      MakeBrighter(rColor.GetColor_Green()),
	      MakeBrighter(rColor.GetColor_Blue()));
   */
   glNormal3f(1,0,0);
   glVertex3f( Vx(+), Vx(-),  Vx(-));  // Boczna x=+1
   glVertex3f( Vx(+), Vx(+),  Vx(-));
   glVertex3f( Vx(+), Vx(+),  Vx(+));
   glVertex3f( Vx(+), Vx(-),  Vx(+));

  glEnd();
}




void DrawPrism()
{
  constexpr int VtxNo = 6;
  constexpr float Radius = 0.5;
  constexpr float AngStep_rad = 2*M_PI/VtxNo;
  constexpr float AngHalf_rad = AngStep_rad/2;
  float  TabX[VtxNo], TabY[VtxNo];
  float  Ang_deg = 0;
  
  glBegin(GL_POLYGON);
   glNormal3f(0.0f,0.0f,1.0f);
   for (int Idx = 0; Idx < VtxNo; ++Idx) {
     glVertex3f(TabX[Idx]=Radius*cos(Ang_deg), TabY[Idx]=Radius*sin(Ang_deg),  0.5f);
     Ang_deg += AngStep_rad;
   }
  glEnd();  

  glBegin(GL_POLYGON);
   glNormal3f(0.0f,0.0f,-1.0f);
   for (int Idx = 0; Idx < VtxNo; ++Idx) {
     glVertex3f(TabX[Idx], TabY[Idx],  -0.5f);
   }
  glEnd();  



  Ang_deg = AngHalf_rad;
  glBegin(GL_QUAD_STRIP);
  
   for (int Idx = 0; Idx < VtxNo; ++Idx) {
     glNormal3f(cos(Ang_deg), sin(Ang_deg), 0.0f);    
     glVertex3f(TabX[Idx], TabY[Idx],  -0.5f);
     glVertex3f(TabX[Idx], TabY[Idx],   0.5f);
     Ang_deg += AngStep_rad;
   }
   glNormal3f(cos(Ang_deg),sin(Ang_deg),0.0f);    
   glVertex3f(TabX[0], TabY[0],  -0.5f);
   glVertex3f(TabX[0], TabY[0],   0.5f);
    
  glEnd();
}






void DrawPyramid()
{
  constexpr int VtxNo = 6;
  constexpr float Radius = 0.5f;
  constexpr float AngStep_rad = 2*M_PI/VtxNo;
  constexpr float AngHalf_rad = AngStep_rad/2;
  float  TabX[VtxNo], TabY[VtxNo];
  float  Ang_deg = 0;
  
  glBegin(GL_POLYGON);
   glNormal3f(0.0f,0.0f,-1.0f);
   for (int Idx = 0; Idx < VtxNo; ++Idx) {
     glVertex3f(TabX[Idx]=Radius*cos(Ang_deg), TabY[Idx]=Radius*sin(Ang_deg),  -0.5f);
     Ang_deg += AngStep_rad;
   }
  glEnd();  

  constexpr float  sin_z = 0.5;
  constexpr float  cos_z = sqrt(3)/2;
  Ang_deg = AngHalf_rad;
  glBegin(GL_TRIANGLE_FAN);
     glVertex3f(0.0f, 0.0f, 0.5f);
     for (int Idx = 0; Idx < VtxNo; ++Idx) {
       glNormal3f(cos(Ang_deg)*cos_z,sin(Ang_deg)*cos_z,sin_z);    
       glVertex3f(TabX[Idx], TabY[Idx],  -0.5f);
       Ang_deg += AngStep_rad;
     }
   glNormal3f(cos(Ang_deg)*cos_z,sin(Ang_deg)*cos_z,sin_z);    
   glVertex3f(TabX[0], TabY[0],  -0.5f);
   glVertex3f(TabX[0], TabY[0],  -0.5f);

 glEnd();
}

//inline
//double Deg2Rad(double Ang_deg) { return M_PI*Ang_deg/180; }




void DrawSphere()
{

  constexpr  int    StripNum = 26; // Musi być parzyste
  constexpr  int    Half_StripNum = StripNum/2;
  constexpr  double Ang_Step_rad = 2*M_PI/StripNum;
    
 float   x_new_z_prev, y_new_z_prev;
 float   x_new_z_new,  y_new_z_new;

 
 float   z_prev = -1, z_new;
 float   radius_prev = 0, radius_new;
 float   sn, cs;

 int Idx_Elev = 0;
 int Idx_Azim;
 for (double Elev_rad = -M_PI/2+Ang_Step_rad; Idx_Elev <= Half_StripNum; ++Idx_Elev, Elev_rad += Ang_Step_rad) {
   z_new = sin(Elev_rad);
   radius_new = cos(Elev_rad);

   
   glBegin(GL_QUAD_STRIP);

   Idx_Azim = 0;
   for (double Azim_rad = 0; Idx_Azim <= StripNum; ++Idx_Azim, Azim_rad += Ang_Step_rad) {

    sn = sin(Azim_rad);  cs = cos(Azim_rad);
    x_new_z_new = radius_new*cs;
    y_new_z_new = radius_new*sn;
    x_new_z_prev = radius_prev*cs;
    y_new_z_prev = radius_prev*sn;


    glNormal3f( x_new_z_prev, y_new_z_prev,  z_prev);
    glVertex3f( x_new_z_prev, y_new_z_prev,  z_prev );

    glNormal3f( x_new_z_new, y_new_z_new,  z_new);
    glVertex3f( x_new_z_new, y_new_z_new,  z_new );     
   }
    
   glEnd();
   
  z_prev = z_new;
  radius_prev = radius_new;
 }
}



void DrawHalfsphere()
{

 constexpr  int    StripNum = 26; // Musi być parzyste
 constexpr  int    Quart_StripNum = StripNum/4; // Aby zachować te same kąty w poziomie i pionie
 constexpr  double Ang_Step_rad = 2*M_PI/StripNum;
    
 float   x_new_z_prev, y_new_z_prev;
 float   x_new_z_new,  y_new_z_new;

 
 float   z_prev = 0, z_new;
 float   radius_prev = 1, radius_new;
 float   sn, cs;

 int Idx_Elev = 0;
 int Idx_Azim;
 for (double Elev_rad = Ang_Step_rad; Idx_Elev <= Quart_StripNum; ++Idx_Elev, Elev_rad += Ang_Step_rad) {
   z_new = sin(Elev_rad);
   radius_new = cos(Elev_rad);

   
   glBegin(GL_QUAD_STRIP);

   Idx_Azim = 0;
   for (double Azim_rad = 0; Idx_Azim <= StripNum; ++Idx_Azim, Azim_rad += Ang_Step_rad) {

    sn = sin(Azim_rad);  cs = cos(Azim_rad);
    x_new_z_new = radius_new*cs;
    y_new_z_new = radius_new*sn;
    x_new_z_prev = radius_prev*cs;
    y_new_z_prev = radius_prev*sn;


    glNormal3f( x_new_z_prev, y_new_z_prev,  z_prev);
    glVertex3f( x_new_z_prev, y_new_z_prev,  z_prev );

    glNormal3f(x_new_z_new, y_new_z_new,  z_new);
    glVertex3f( x_new_z_new, y_new_z_new,  z_new );     
   }
    
   glEnd();
   
  z_prev = z_new;
  radius_prev = radius_new;
 }
 /*
  constexpr float  sin_z = sin(M_PI/2-Ang_Step_rad);
  constexpr float  cos_z = cos(M_PI/2-Ang_Step_rad);

  glBegin(GL_TRIANGLE_FAN);
    glVertex3f(0.0f, 0.0f, 1.0f);
    Idx_Azim = 0;    
    for (double Azim_rad = 0; Idx_Azim <= StripNum; ++Idx_Azim, Azim_rad += Ang_Step_rad) {
       sn = sin(Azim_rad);  cs = cos(Azim_rad);
       glNormal3f(cos(Ang_deg)*cos_z,sin(Ang_deg)*cos_z,sin_z);    
       glVertex3f(TabX[Idx], TabY[Idx],  -0.5f);
       Ang_deg += AngStep_rad;
     }
   glNormal3f(cos(Azim_rad)*cos_z,sin(Azim_rad)*cos_z,sin_z);    
   glVertex3f(0, TabY[0],  -0.5f);
   glVertex3f(TabX[0], TabY[0],  -0.5f);
 */
 glEnd();

}





void gsv::Cuboid::DrawGL() const
{
  const Vector3d   &rPos_m = GetPosition_m();
  const RGB        &rColor = GetRGB();
  //  Vector3d         Size_m = GetSize_m();
  const gsv::Vector3d  &rScale=GetScale();
  const gsv::Vector3d  &rShift=GetShift_bsc();
  //  Size_m /= 2;

  
  glTranslatef(COORDS3(rPos_m));  
  glRotatef(GetAng_Yaw_deg(), 0.0f, 0.0f, 1.0f);
  glRotatef(GetAng_Pitch_deg(), 0.0f, 1.0f, 0.0f);
  glRotatef(GetAng_Roll_deg(), 1.0f, 0.0f, 0.0f);
  glScalef(COORDS3(rScale));  
  glTranslatef(COORDS3(rShift));

  glColor3ub(rColor.GetColor_Red(),rColor.GetColor_Green(),rColor.GetColor_Blue());  

  switch (GetObjType()) {
    case gsv::CuboidType:  DrawCuboid(); break;
    case gsv::PrismType:   DrawPrism(); break;
    case gsv::PyramidType: DrawPyramid(); break;
    case gsv::SphereType:    DrawSphere(); break;
    case gsv::HemisphereType:  DrawHalfsphere(); break;
  }

  glTranslatef(OPO_COORDS3(rShift));   
  glScalef(REV_COORDS3(rScale));
}





#define DISP_COOR( vec, index ) setw(4) << vec[index] << " "
#define DISP_VECT( vec ) DISP_COOR(vec,0) <<  DISP_COOR(vec,1) <<  DISP_COOR(vec,2)

std::string gsv::Cuboid::BuildCmd() const
{
  ostringstream  OStrm;

  OStrm << "UpdateObj "// <<  DISP_VECT(GetSize_m()) << "  "
	<< DISP_VECT(GetPosition_m()) << "  "
	<< GetAng_Roll_deg() << " "
	<< GetAng_Pitch_deg() << " "
	<< GetAng_Yaw_deg() << "  "
	<< GetColorRGB();
  return OStrm.str();
}
