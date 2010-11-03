/// autogenerated analytical inverse kinematics code from ikfast program
/// \author Rosen Diankov
///
/// Licensed under the Apache License, Version 2.0 (the "License");
/// you may not use this file except in compliance with the License.
/// You may obtain a copy of the License at
///     http://www.apache.org/licenses/LICENSE-2.0
/// 
/// Unless required by applicable law or agreed to in writing, software
/// distributed under the License is distributed on an "AS IS" BASIS,
/// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
/// See the License for the specific language governing permissions and
/// limitations under the License.
///
/// generated 2010-11-03 15:41:25.070979
/// To compile with gcc:
///     gcc -lstdc++ ik.cpp
/// To compile without any main function as a shared object:
///     gcc -fPIC -lstdc++ -DIKFAST_NO_MAIN -shared -Wl,-soname,ik.so -o ik.so ik.cpp
#include <cmath>
#include <cassert>
#include <vector>
#include <limits>
#include <algorithm>
#include <complex>

#define IK2PI  6.28318530717959
#define IKPI  3.14159265358979
#define IKPI_2  1.57079632679490

#ifdef _MSC_VER
#ifndef isnan
#define isnan _isnan
#endif
#endif // _MSC_VER

// defined when creating a shared object/dll
#ifdef IKFAST_CLIBRARY
#ifdef _MSC_VER
#define IKFAST_API extern "C" __declspec(dllexport)
#else
#define IKFAST_API extern "C"
#endif
#else
#define IKFAST_API
#endif

typedef double IKReal;
class IKSolution
{
public:
    /// Gets a solution given its free parameters
    /// \param pfree The free parameters required, range is in [-pi,pi]
    void GetSolution(IKReal* psolution, const IKReal* pfree) const {
        for(size_t i = 0; i < basesol.size(); ++i) {
            if( basesol[i].freeind < 0 )
                psolution[i] = basesol[i].foffset;
            else {
                assert(pfree != NULL);
                psolution[i] = pfree[basesol[i].freeind]*basesol[i].fmul + basesol[i].foffset;
                if( psolution[i] > IKPI )
                    psolution[i] -= IK2PI;
                else if( psolution[i] < -IKPI )
                    psolution[i] += IK2PI;
            }
        }
    }

    /// Gets the free parameters the solution requires to be set before a full solution can be returned
    /// \return vector of indices indicating the free parameters
    const std::vector<int>& GetFree() const { return vfree; }

    struct VARIABLE
    {
        VARIABLE() : freeind(-1), fmul(0), foffset(0) {}
        VARIABLE(int freeind, IKReal fmul, IKReal foffset) : freeind(freeind), fmul(fmul), foffset(foffset) {}
        int freeind;
        IKReal fmul, foffset; ///< joint value is fmul*sol[freeind]+foffset
    };

    std::vector<VARIABLE> basesol;       ///< solution and their offsets if joints are mimiced
    std::vector<int> vfree;
};

inline float IKabs(float f) { return fabsf(f); }
inline double IKabs(double f) { return fabs(f); }

inline float IKlog(float f) { return logf(f); }
inline double IKlog(double f) { return log(f); }

inline float IKasin(float f)
{
assert( f > -1.001f && f < 1.001f ); // any more error implies something is wrong with the solver
if( f <= -1 ) return -IKPI_2;
else if( f >= 1 ) return IKPI_2;
return asinf(f);
}
inline double IKasin(double f)
{
assert( f > -1.001 && f < 1.001 ); // any more error implies something is wrong with the solver
if( f <= -1 ) return -IKPI_2;
else if( f >= 1 ) return IKPI_2;
return asin(f);
}

// return positive value in [0,y)
inline float IKfmod(float x, float y)
{
    while(x < 0) {
        x += y;
    }
    return fmodf(x,y);
}

// return positive value in [0,y)
inline float IKfmod(double x, double y)
{
    while(x < 0) {
        x += y;
    }
    return fmod(x,y);
}

inline float IKacos(float f)
{
assert( f > -1.001f && f < 1.001f ); // any more error implies something is wrong with the solver
if( f <= -1 ) return IKPI;
else if( f >= 1 ) return 0.0f;
return acosf(f);
}
inline double IKacos(double f)
{
assert( f > -1.001 && f < 1.001 ); // any more error implies something is wrong with the solver
if( f <= -1 ) return IKPI;
else if( f >= 1 ) return 0.0;
return acos(f);
}
inline float IKsin(float f) { return sinf(f); }
inline double IKsin(double f) { return sin(f); }
inline float IKcos(float f) { return cosf(f); }
inline double IKcos(double f) { return cos(f); }
inline float IKsqrt(float f) { if( f <= 0.0f ) return 0.0f; return sqrtf(f); }
inline double IKsqrt(double f) { if( f <= 0.0 ) return 0.0; return sqrt(f); }
inline float IKatan2(float fy, float fx) {
    if( isnan(fy) ) {
        assert(!isnan(fx)); // if both are nan, probably wrong value will be returned
        return IKPI_2;
    }
    else if( isnan(fx) )
        return 0;
    return atan2f(fy,fx);
}
inline double IKatan2(double fy, double fx) {
    if( isnan(fy) ) {
        assert(!isnan(fx)); // if both are nan, probably wrong value will be returned
        return IKPI_2;
    }
    else if( isnan(fx) )
        return 0;
    return atan2(fy,fx);
}

IKFAST_API int getNumFreeParameters() { return 0; }
IKFAST_API int* getFreeParameters() { return NULL; }
IKFAST_API int getNumJoints() { return 6; }

IKFAST_API int getIKRealSize() { return sizeof(IKReal); }

IKFAST_API int getIKType() { return 1; }

/// solves the forward kinematics equations.
/// \param pfree is an array specifying the free joints of the chain.
IKFAST_API void fk(const IKReal* j, IKReal* eetrans, IKReal* eerot) {
    IKReal x0,x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11;
    x0=IKcos(j[0]);
    x1=IKcos(j[3]);
    x2=IKsin(j[5]);
    x3=IKcos(j[1]);
    x4=IKcos(j[5]);
    x5=IKsin(j[2]);
    x6=IKcos(j[4]);
    x7=IKsin(j[0]);
    x8=IKsin(j[3]);
    x9=IKsin(j[1]);
    x10=IKcos(j[2]);
    x11=IKsin(j[4]);
    eerot[0]=((((-1.00000000000000)*(x0)*(x10)*(x11)*(x4)*(x9)))+(((-1.00000000000000)*(x1)*(x2)*(x7)))+(((-1.00000000000000)*(x0)*(x10)*(x2)*(x3)*(x8)))+(((x0)*(x2)*(x5)*(x8)*(x9)))+(((-1.00000000000000)*(x0)*(x1)*(x4)*(x5)*(x6)*(x9)))+(((-1.00000000000000)*(x4)*(x6)*(x7)*(x8)))+(((x0)*(x1)*(x10)*(x3)*(x4)*(x6)))+(((-1.00000000000000)*(x0)*(x11)*(x3)*(x4)*(x5))));
    eerot[1]=((((x2)*(x6)*(x7)*(x8)))+(((-1.00000000000000)*(x0)*(x1)*(x10)*(x2)*(x3)*(x6)))+(((x0)*(x4)*(x5)*(x8)*(x9)))+(((x0)*(x10)*(x11)*(x2)*(x9)))+(((-1.00000000000000)*(x1)*(x4)*(x7)))+(((x0)*(x11)*(x2)*(x3)*(x5)))+(((x0)*(x1)*(x2)*(x5)*(x6)*(x9)))+(((-1.00000000000000)*(x0)*(x10)*(x3)*(x4)*(x8))));
    eerot[2]=((((x0)*(x10)*(x6)*(x9)))+(((-1.00000000000000)*(x0)*(x1)*(x11)*(x5)*(x9)))+(((x0)*(x1)*(x10)*(x11)*(x3)))+(((-1.00000000000000)*(x11)*(x7)*(x8)))+(((x0)*(x3)*(x5)*(x6))));
    eetrans[0]=((((-0.175000000000000)*(x0)*(x1)*(x11)*(x5)*(x9)))+(((0.0203000000000000)*(x0)*(x5)*(x9)))+(((-0.0203000000000000)*(x0)*(x10)*(x3)))+(((0.175000000000000)*(x0)*(x10)*(x6)*(x9)))+(((-0.175000000000000)*(x11)*(x7)*(x8)))+(((0.175000000000000)*(x0)*(x1)*(x10)*(x11)*(x3)))+(((0.431800000000000)*(x0)*(x3)))+(((0.433100000000000)*(x0)*(x3)*(x5)))+(((0.433100000000000)*(x0)*(x10)*(x9)))+(((0.175000000000000)*(x0)*(x3)*(x5)*(x6)))+(((-0.150100000000000)*(x7))));
    eerot[3]=((((-1.00000000000000)*(x1)*(x4)*(x5)*(x6)*(x7)*(x9)))+(((x2)*(x5)*(x7)*(x8)*(x9)))+(((-1.00000000000000)*(x11)*(x3)*(x4)*(x5)*(x7)))+(((-1.00000000000000)*(x10)*(x2)*(x3)*(x7)*(x8)))+(((x1)*(x10)*(x3)*(x4)*(x6)*(x7)))+(((x0)*(x4)*(x6)*(x8)))+(((-1.00000000000000)*(x10)*(x11)*(x4)*(x7)*(x9)))+(((x0)*(x1)*(x2))));
    eerot[4]=((((x1)*(x2)*(x5)*(x6)*(x7)*(x9)))+(((x10)*(x11)*(x2)*(x7)*(x9)))+(((-1.00000000000000)*(x10)*(x3)*(x4)*(x7)*(x8)))+(((x11)*(x2)*(x3)*(x5)*(x7)))+(((-1.00000000000000)*(x1)*(x10)*(x2)*(x3)*(x6)*(x7)))+(((x4)*(x5)*(x7)*(x8)*(x9)))+(((-1.00000000000000)*(x0)*(x2)*(x6)*(x8)))+(((x0)*(x1)*(x4))));
    eerot[5]=((((x10)*(x6)*(x7)*(x9)))+(((x1)*(x10)*(x11)*(x3)*(x7)))+(((-1.00000000000000)*(x1)*(x11)*(x5)*(x7)*(x9)))+(((x3)*(x5)*(x6)*(x7)))+(((x0)*(x11)*(x8))));
    eetrans[1]=((((0.175000000000000)*(x0)*(x11)*(x8)))+(((0.175000000000000)*(x10)*(x6)*(x7)*(x9)))+(((0.175000000000000)*(x1)*(x10)*(x11)*(x3)*(x7)))+(((0.175000000000000)*(x3)*(x5)*(x6)*(x7)))+(((0.0203000000000000)*(x5)*(x7)*(x9)))+(((0.433100000000000)*(x10)*(x7)*(x9)))+(((0.150100000000000)*(x0)))+(((-0.0203000000000000)*(x10)*(x3)*(x7)))+(((-0.175000000000000)*(x1)*(x11)*(x5)*(x7)*(x9)))+(((0.431800000000000)*(x3)*(x7)))+(((0.433100000000000)*(x3)*(x5)*(x7))));
    eerot[6]=((((-1.00000000000000)*(x1)*(x10)*(x4)*(x6)*(x9)))+(((x11)*(x4)*(x5)*(x9)))+(((-1.00000000000000)*(x1)*(x3)*(x4)*(x5)*(x6)))+(((-1.00000000000000)*(x10)*(x11)*(x3)*(x4)))+(((x10)*(x2)*(x8)*(x9)))+(((x2)*(x3)*(x5)*(x8))));
    eerot[7]=((((x3)*(x4)*(x5)*(x8)))+(((-1.00000000000000)*(x11)*(x2)*(x5)*(x9)))+(((x10)*(x11)*(x2)*(x3)))+(((x10)*(x4)*(x8)*(x9)))+(((x1)*(x10)*(x2)*(x6)*(x9)))+(((x1)*(x2)*(x3)*(x5)*(x6))));
    eerot[8]=((((x10)*(x3)*(x6)))+(((-1.00000000000000)*(x1)*(x10)*(x11)*(x9)))+(((-1.00000000000000)*(x5)*(x6)*(x9)))+(((-1.00000000000000)*(x1)*(x11)*(x3)*(x5))));
    eetrans[2]=((1.37100000000000)+(((-0.175000000000000)*(x5)*(x6)*(x9)))+(((-0.433100000000000)*(x5)*(x9)))+(((-0.431800000000000)*(x9)))+(((-0.175000000000000)*(x1)*(x10)*(x11)*(x9)))+(((-0.175000000000000)*(x1)*(x11)*(x3)*(x5)))+(((0.0203000000000000)*(x10)*(x9)))+(((0.433100000000000)*(x10)*(x3)))+(((0.0203000000000000)*(x3)*(x5)))+(((0.175000000000000)*(x10)*(x3)*(x6))));
}

class IKSolver {
public:
IKReal j0, cj0, sj0,
j1, cj1, sj1,
j2, cj2, sj2,
j3, cj3, sj3,
j4, cj4, sj4,
j5, cj5, sj5,
new_r00, r00,
new_r01, r01,
new_r02, r02,
new_r10, r10,
new_r11, r11,
new_r12, r12,
new_r20, r20,
new_r21, r21,
new_r22, r22,
new_px, new_py, new_pz, px, py, pz;

bool ik(const IKReal* eetrans, const IKReal* eerot, const IKReal* pfree, std::vector<IKSolution>& vsolutions) {
for(int dummyiter = 0; dummyiter < 1; ++dummyiter) {
    vsolutions.resize(0); vsolutions.reserve(8);
    r00 = eerot[0*3+0];
    r01 = eerot[0*3+1];
    r02 = eerot[0*3+2];
    r10 = eerot[1*3+0];
    r11 = eerot[1*3+1];
    r12 = eerot[1*3+2];
    r20 = eerot[2*3+0];
    r21 = eerot[2*3+1];
    r22 = eerot[2*3+2];
    px = eetrans[0]; py = eetrans[1]; pz = eetrans[2];
    
    new_r00=r00;
    new_r01=r01;
    new_r02=r02;
    new_px=((((-0.175000000000000)*(r02)))+(px));
    new_r10=r10;
    new_r11=r11;
    new_r12=r12;
    new_py=((((-0.175000000000000)*(r12)))+(py));
    new_r20=r20;
    new_r21=r21;
    new_r22=r22;
    new_pz=((-1.37100000000000)+(((-0.175000000000000)*(r22)))+(pz));
    r00 = new_r00; r01 = new_r01; r02 = new_r02; r10 = new_r10; r11 = new_r11; r12 = new_r12; r20 = new_r20; r21 = new_r21; r22 = new_r22; px = new_px; py = new_py; pz = new_pz;
    {
        if( 1 )
        {
        {
        IKReal j2array[2], cj2array[2], sj2array[2];
        bool j2valid[2]={false};
        IKReal x0=(py)*(py);
        IKReal x1=((2.67068488900329)*(x0));
        IKReal x2=(pz)*(pz);
        IKReal x3=((2.67068488900329)*(x2));
        IKReal x4=(px)*(px);
        IKReal x5=((2.67068488900329)*(x4));
        IKReal x6=((-1.06017897616850)+(x3)+(x1)+(x5));
        if( (x6) < -1.0001 || (x6) > 1.0001 )
            continue;
        IKReal x7=IKasin(x6);
        j2array[0]=((0.0468371131132389)+(x7));
        sj2array[0]=IKsin(j2array[0]);
        cj2array[0]=IKcos(j2array[0]);
        j2array[1]=((3.18842976670303)+(((-1.00000000000000)*(x7))));
        sj2array[1]=IKsin(j2array[1]);
        cj2array[1]=IKcos(j2array[1]);
        if( j2array[0] > IKPI )
            j2array[0]-=IK2PI;
        else if( j2array[0] < -IKPI )
            j2array[0]+=IK2PI;
        j2valid[0] = true;
        if( j2array[1] > IKPI )
            j2array[1]-=IK2PI;
        else if( j2array[1] < -IKPI )
            j2array[1]+=IK2PI;
        j2valid[1] = true;
        if( j2valid[0] && j2valid[1] && IKabs(cj2array[0]-cj2array[1]) < 0.0001 && IKabs(sj2array[0]-sj2array[1]) < 0.0001 )
            j2valid[1]=false;
        for(int ij2 = 0; ij2 < 2; ++ij2)
        {
        if( !j2valid[ij2] ) { continue; }
        j2 = j2array[ij2]; cj2 = cj2array[ij2]; sj2 = sj2array[ij2];
        
        {
        IKReal j0array[2], cj0array[2], sj0array[2];
        bool j0valid[2]={false};
        IKReal x8=x4;
        IKReal x9=x0;
        IKReal x10=((x9)+(x8));
        if( (x10) < (IKReal)-0.00001 )
            continue;
        IKReal x11=IKsqrt(x10);
        IKReal x12=IKabs(x11);
        IKReal x13=((IKabs(x12) != 0)?((IKReal)1/(x12)):(IKReal)1.0e30);
        IKReal x14=((0.150100000000000)*(x13));
        if( (x14) < -1.0001 || (x14) > 1.0001 )
            continue;
        IKReal x15=IKasin(x14);
        IKReal x16=((-1.00000000000000)*(py));
        IKReal x17=IKatan2(x16, px);
        j0array[0]=((((-1.00000000000000)*(x17)))+(((-1.00000000000000)*(x15))));
        sj0array[0]=IKsin(j0array[0]);
        cj0array[0]=IKcos(j0array[0]);
        j0array[1]=((3.14159265358979)+(((-1.00000000000000)*(x17)))+(x15));
        sj0array[1]=IKsin(j0array[1]);
        cj0array[1]=IKcos(j0array[1]);
        if( j0array[0] > IKPI )
            j0array[0]-=IK2PI;
        else if( j0array[0] < -IKPI )
            j0array[0]+=IK2PI;
        j0valid[0] = true;
        if( j0array[1] > IKPI )
            j0array[1]-=IK2PI;
        else if( j0array[1] < -IKPI )
            j0array[1]+=IK2PI;
        j0valid[1] = true;
        if( j0valid[0] && j0valid[1] && IKabs(cj0array[0]-cj0array[1]) < 0.0001 && IKabs(sj0array[0]-sj0array[1]) < 0.0001 )
            j0valid[1]=false;
        for(int ij0 = 0; ij0 < 2; ++ij0)
        {
        if( !j0valid[ij0] ) { continue; }
        j0 = j0array[ij0]; cj0 = cj0array[ij0]; sj0 = sj0array[ij0];
        
        {
            IKReal j1eval[1];
            j1eval[0]=((((-0.433100000000000)*(cj0)*(cj2)*(px)))+(((-0.0203000000000000)*(py)*(sj0)*(sj2)))+(((-0.0203000000000000)*(cj0)*(px)*(sj2)))+(((-0.433100000000000)*(pz)*(sj2)))+(((-0.431800000000000)*(pz)))+(((0.0203000000000000)*(cj2)*(pz)))+(((-0.433100000000000)*(cj2)*(py)*(sj0))));
            if( IKabs(j1eval[0]) < 0.000001  )
            {
                {
                    if( 1 )
                    {
                    {
                        IKReal j1eval[1];
                        j1eval[0]=((((-1.00000000000000)*((cj0)*(cj0))*((px)*(px))))+(((-1.00000000000000)*((pz)*(pz))))+(((-2.00000000000000)*(cj0)*(px)*(py)*(sj0)))+(((-1.00000000000000)*((py)*(py))*((sj0)*(sj0)))));
                        if( IKabs(j1eval[0]) < 0.000001  )
                        {
                            {
                                if( 1 )
                                {
                                {
                                    IKReal j1eval[1];
                                    j1eval[0]=((((-0.0203000000000000)*(cj2)*(pz)))+(((0.431800000000000)*(pz)))+(((0.433100000000000)*(pz)*(sj2)))+(((0.433100000000000)*(cj2)*(py)*(sj0)))+(((0.433100000000000)*(cj0)*(cj2)*(px)))+(((0.0203000000000000)*(cj0)*(px)*(sj2)))+(((0.0203000000000000)*(py)*(sj0)*(sj2))));
                                    if( IKabs(j1eval[0]) < 0.000001  )
                                    {
                                        {
                                            if( 1 )
                                            {
                                            continue;
                                        
                                        } else
                                        {
                                        }
                                        }
                                    
                                    } else
                                    {
                                        {
                                        IKReal j1array[1], cj1array[1], sj1array[1];
                                        bool j1valid[1]={false};
                                        IKReal x18=((cj0)*(px));
                                        IKReal x19=((py)*(sj0));
                                        IKReal x20=((x19)+(x18));
                                        IKReal x21=((-1.00000000000000)*(x20));
                                        IKReal x22=((0.433100000000000)*(sj2));
                                        IKReal x23=((0.431800000000000)+(x22));
                                        IKReal x24=((0.0203000000000000)*(cj2));
                                        IKReal x25=((((-1.00000000000000)*(x24)))+(x23));
                                        IKReal x26=((0.431800000000000)*(pz));
                                        IKReal x27=((1.00000000000000)*(pz)*(x22));
                                        IKReal x28=((0.433100000000000)*(cj2)*(x19));
                                        IKReal x29=((0.433100000000000)*(cj2)*(x18));
                                        IKReal x30=((0.0203000000000000)*(sj2)*(x18));
                                        IKReal x31=((0.0203000000000000)*(sj2)*(x19));
                                        IKReal x32=((x31)+(x30)+(x26)+(x27)+(x28)+(x29));
                                        IKReal x33=((pz)*(x24));
                                        IKReal x34=((((-1.00000000000000)*(x33)))+(x32));
                                        IKReal x35=((IKabs(x34) != 0)?((IKReal)1/(x34)):(IKReal)1.0e30);
                                        j1array[0]=IKatan2(((x35)*((((x21)*(x21))+(((-1.00000000000000)*((x25)*(x25))))))), ((x35)*(((((-1.00000000000000)*(pz)*(x21)))+(((x25)*(((((0.0203000000000000)*(sj2)))+(((0.433100000000000)*(cj2)))))))))));
                                        sj1array[0]=IKsin(j1array[0]);
                                        cj1array[0]=IKcos(j1array[0]);
                                        if( j1array[0] > IKPI )
                                            j1array[0]-=IK2PI;
                                        else if( j1array[0] < -IKPI )
                                            j1array[0]+=IK2PI;
                                        j1valid[0] = true;
                                        for(int ij1 = 0; ij1 < 1; ++ij1)
                                        {
                                        if( !j1valid[ij1] ) { continue; }
                                        j1 = j1array[ij1]; cj1 = cj1array[ij1]; sj1 = sj1array[ij1];
                                        
                                        rotationfunction0(vsolutions);
                                        }
                                        }
                                    
                                    }
                                
                                }
                            
                            } else
                            {
                            }
                            }
                        
                        } else
                        {
                            {
                            IKReal j1array[1], cj1array[1], sj1array[1];
                            bool j1valid[1]={false};
                            IKReal x62=((0.0203000000000000)*(sj2));
                            IKReal x63=((0.433100000000000)*(cj2));
                            IKReal x64=((x62)+(x63));
                            IKReal x65=((cj0)*(px));
                            IKReal x66=((py)*(sj0));
                            IKReal x67=((x65)+(x66));
                            IKReal x68=((-1.00000000000000)*(x67));
                            IKReal x69=((0.433100000000000)*(sj2));
                            IKReal x70=((0.431800000000000)+(x69));
                            IKReal x71=((0.0203000000000000)*(cj2));
                            IKReal x72=((((-1.00000000000000)*(x71)))+(x70));
                            IKReal x73=x0;
                            IKReal x74=(sj0)*(sj0);
                            IKReal x75=(x66)*(x66);
                            IKReal x76=((2.00000000000000)*(x65)*(x66));
                            IKReal x77=x2;
                            IKReal x78=(cj0)*(cj0);
                            IKReal x79=x4;
                            IKReal x80=(x65)*(x65);
                            IKReal x81=((x77)+(x76)+(x75)+(x80));
                            IKReal x82=((-1.00000000000000)*(x81));
                            IKReal x83=((IKabs(x82) != 0)?((IKReal)1/(x82)):(IKReal)1.0e30);
                            j1array[0]=IKatan2(((x83)*(((((pz)*(x72)))+(((x64)*(x68)))))), ((x83)*(((((x68)*(x72)))+(((-1.00000000000000)*(pz)*(x64)))))));
                            sj1array[0]=IKsin(j1array[0]);
                            cj1array[0]=IKcos(j1array[0]);
                            if( j1array[0] > IKPI )
                                j1array[0]-=IK2PI;
                            else if( j1array[0] < -IKPI )
                                j1array[0]+=IK2PI;
                            j1valid[0] = true;
                            for(int ij1 = 0; ij1 < 1; ++ij1)
                            {
                            if( !j1valid[ij1] ) { continue; }
                            j1 = j1array[ij1]; cj1 = cj1array[ij1]; sj1 = sj1array[ij1];
                            
                            rotationfunction0(vsolutions);
                            }
                            }
                        
                        }
                    
                    }
                
                } else
                {
                }
                }
            
            } else
            {
                {
                IKReal j1array[1], cj1array[1], sj1array[1];
                bool j1valid[1]={false};
                IKReal x84=((0.0203000000000000)*(sj2));
                IKReal x85=((0.433100000000000)*(cj2));
                IKReal x86=((x84)+(x85));
                IKReal x87=((0.0203000000000000)*(cj2)*(pz));
                IKReal x88=((0.431800000000000)*(pz));
                IKReal x89=((0.433100000000000)*(pz)*(sj2));
                IKReal x90=((1.00000000000000)*(py)*(sj0)*(x85));
                IKReal x91=((1.00000000000000)*(cj0)*(px)*(x85));
                IKReal x92=((cj0)*(px)*(x84));
                IKReal x93=((py)*(sj0)*(x84));
                IKReal x94=((x88)+(x89)+(x91)+(x90)+(x93)+(x92));
                IKReal x95=((x87)+(((-1.00000000000000)*(x94))));
                IKReal x96=((IKabs(x95) != 0)?((IKReal)1/(x95)):(IKReal)1.0e30);
                j1array[0]=IKatan2(((x96)*(((((-1.00000000000000)*((x86)*(x86))))+((pz)*(pz))))), ((x96)*(((((pz)*(((((-1.00000000000000)*(py)*(sj0)))+(((-1.00000000000000)*(cj0)*(px)))))))+(((-1.00000000000000)*(x86)*(((0.431800000000000)+(((0.433100000000000)*(sj2)))+(((-0.0203000000000000)*(cj2)))))))))));
                sj1array[0]=IKsin(j1array[0]);
                cj1array[0]=IKcos(j1array[0]);
                if( j1array[0] > IKPI )
                    j1array[0]-=IK2PI;
                else if( j1array[0] < -IKPI )
                    j1array[0]+=IK2PI;
                j1valid[0] = true;
                for(int ij1 = 0; ij1 < 1; ++ij1)
                {
                if( !j1valid[ij1] ) { continue; }
                j1 = j1array[ij1]; cj1 = cj1array[ij1]; sj1 = sj1array[ij1];
                
                rotationfunction0(vsolutions);
                }
                }
            
            }
        
        }
        }
        }
        }
        }
    
    } else
    {
    }
    }
}
return vsolutions.size()>0;
}
    inline void rotationfunction0(std::vector<IKSolution>& vsolutions) {
        IKReal x36=((cj0)*(cj1)*(cj2));
        IKReal x37=((cj0)*(sj1)*(sj2));
        IKReal x38=((((-1.00000000000000)*(x37)))+(x36));
        IKReal x39=((cj2)*(sj1));
        IKReal x40=((cj1)*(sj2));
        IKReal x41=((x39)+(x40));
        IKReal x42=((-1.00000000000000)*(x41));
        IKReal x43=((cj1)*(cj2)*(sj0));
        IKReal x44=((sj0)*(sj1)*(sj2));
        IKReal x45=((((-1.00000000000000)*(x44)))+(x43));
        IKReal x46=((cj1)*(cj2));
        IKReal x47=((sj1)*(sj2));
        IKReal x48=((((-1.00000000000000)*(x47)))+(x46));
        IKReal x49=((sj0)*(x40));
        IKReal x50=((sj0)*(x39));
        IKReal x51=((x49)+(x50));
        IKReal x52=((cj0)*(x39));
        IKReal x53=((cj0)*(x40));
        IKReal x54=((x53)+(x52));
        new_r00=((((r20)*(x42)))+(((r00)*(x38)))+(((r10)*(x45))));
        new_r01=((((r01)*(x38)))+(((r11)*(x45)))+(((r21)*(x42))));
        new_r02=((((r22)*(x42)))+(((r02)*(x38)))+(((r12)*(x45))));
        new_r10=((((-1.00000000000000)*(r00)*(sj0)))+(((cj0)*(r10))));
        new_r11=((((-1.00000000000000)*(r01)*(sj0)))+(((cj0)*(r11))));
        new_r12=((((cj0)*(r12)))+(((-1.00000000000000)*(r02)*(sj0))));
        new_r20=((((r20)*(x48)))+(((r10)*(x51)))+(((r00)*(x54))));
        new_r21=((((r11)*(x51)))+(((r01)*(x54)))+(((r21)*(x48))));
        new_r22=((((r02)*(x54)))+(((r22)*(x48)))+(((r12)*(x51))));
        {
        IKReal j4eval;
        j4eval=new_r22;
        if( j4eval >= -1.000010 && j4eval <= -0.999990 )
        {
            {
            j4 = 3.141593; sj4 = 0.000000; cj4 = -1.000000;
            IKReal j3mul = 1;
            j3=0;
            IKReal j5mul=1.00000000000000;
            j5=((-1.00000000000000)*(IKatan2(((-1.00000000000000)*(new_r10)), new_r11)));
            vsolutions.push_back(IKSolution()); IKSolution& solution = vsolutions.back();
            solution.basesol.resize(6);
            solution.basesol[0].foffset = j0;
            solution.basesol[1].foffset = j1;
            solution.basesol[2].foffset = j2;
            solution.basesol[3].foffset = j3;
            solution.basesol[3].fmul = j3mul;
            solution.basesol[3].freeind = 0;
            solution.basesol[4].foffset = j4;
            solution.basesol[5].foffset = j5;
            solution.basesol[5].fmul = j5mul;
            solution.basesol[5].freeind = 0;
            solution.vfree.resize(1);
            solution.vfree[0] = 3;
            }
        
        } else
        if( j4eval >= 0.999990 && j4eval <= 1.000010 )
        {
            {
            j4 = 0.000000; sj4 = 0.000000; cj4 = 1.000000;
            IKReal j3mul = 1;
            j3=0;
            IKReal j5mul=-1.00000000000000;
            j5=((((-1.00000000000000)*(IKatan2(new_r10, ((-1.00000000000000)*(new_r11))))))+(((-1.00000000000000)*(3.14159265358979))));
            vsolutions.push_back(IKSolution()); IKSolution& solution = vsolutions.back();
            solution.basesol.resize(6);
            solution.basesol[0].foffset = j0;
            solution.basesol[1].foffset = j1;
            solution.basesol[2].foffset = j2;
            solution.basesol[3].foffset = j3;
            solution.basesol[3].fmul = j3mul;
            solution.basesol[3].freeind = 0;
            solution.basesol[4].foffset = j4;
            solution.basesol[5].foffset = j5;
            solution.basesol[5].fmul = j5mul;
            solution.basesol[5].freeind = 0;
            solution.vfree.resize(1);
            solution.vfree[0] = 3;
            }
        
        } else
        {
            {
            IKReal j3array[2], cj3array[2], sj3array[2];
            bool j3valid[2]={false};
            j3array[0]=IKatan2(new_r12, new_r02);
            sj3array[0]=IKsin(j3array[0]);
            cj3array[0]=IKcos(j3array[0]);
            j3array[1] = j3array[0] > 0 ? j3array[0]-IKPI : j3array[0]+IKPI;
            sj3array[1] = -sj3array[0];
            cj3array[1] = -cj3array[0];
            if( j3array[0] > IKPI )
                j3array[0]-=IK2PI;
            else if( j3array[0] < -IKPI )
                j3array[0]+=IK2PI;
            j3valid[0] = true;
            if( j3array[1] > IKPI )
                j3array[1]-=IK2PI;
            else if( j3array[1] < -IKPI )
                j3array[1]+=IK2PI;
            j3valid[1] = true;
            if( j3valid[0] && j3valid[1] && IKabs(cj3array[0]-cj3array[1]) < 0.0001 && IKabs(sj3array[0]-sj3array[1]) < 0.0001 )
                j3valid[1]=false;
            for(int ij3 = 0; ij3 < 2; ++ij3)
            {
            if( !j3valid[ij3] ) { continue; }
            j3 = j3array[ij3]; cj3 = cj3array[ij3]; sj3 = sj3array[ij3];
            
            {
            IKReal evalcond[1];
                evalcond[0]=IKsin(j3);
                if( IKabs(evalcond[0]) < 0.000010  )
                {
                {
                IKReal j5array[1], cj5array[1], sj5array[1];
                bool j5valid[1]={false};
                IKReal x55=IKcos(j3);
                IKReal x56=((IKabs(x55) != 0)?((IKReal)1/(x55)):(IKReal)1.0e30);
                j5array[0]=IKatan2(((new_r10)*(x56)), ((new_r11)*(x56)));
                sj5array[0]=IKsin(j5array[0]);
                cj5array[0]=IKcos(j5array[0]);
                if( j5array[0] > IKPI )
                    j5array[0]-=IK2PI;
                else if( j5array[0] < -IKPI )
                    j5array[0]+=IK2PI;
                j5valid[0] = true;
                for(int ij5 = 0; ij5 < 1; ++ij5)
                {
                if( !j5valid[ij5] ) { continue; }
                j5 = j5array[ij5]; cj5 = cj5array[ij5]; sj5 = sj5array[ij5];
                
                {
                IKReal j4array[1], cj4array[1], sj4array[1];
                bool j4valid[1]={false};
                j4array[0]=IKatan2(((new_r02)*(((IKabs(IKcos(j3)) != 0)?((IKReal)1/(IKcos(j3))):(IKReal)1.0e30))), new_r22);
                sj4array[0]=IKsin(j4array[0]);
                cj4array[0]=IKcos(j4array[0]);
                if( j4array[0] > IKPI )
                    j4array[0]-=IK2PI;
                else if( j4array[0] < -IKPI )
                    j4array[0]+=IK2PI;
                j4valid[0] = true;
                for(int ij4 = 0; ij4 < 1; ++ij4)
                {
                if( !j4valid[ij4] ) { continue; }
                j4 = j4array[ij4]; cj4 = cj4array[ij4]; sj4 = sj4array[ij4];
                
                vsolutions.push_back(IKSolution()); IKSolution& solution = vsolutions.back();
                solution.basesol.resize(6);
                solution.basesol[0].foffset = j0;
                solution.basesol[1].foffset = j1;
                solution.basesol[2].foffset = j2;
                solution.basesol[3].foffset = j3;
                solution.basesol[4].foffset = j4;
                solution.basesol[5].foffset = j5;
                solution.vfree.resize(0);
                }
                }
                }
                }
            
            } else
            {
                if( 1 )
                {
                {
                IKReal j4array[1], cj4array[1], sj4array[1];
                bool j4valid[1]={false};
                j4array[0]=IKatan2(((new_r12)*(((IKabs(sj3) != 0)?((IKReal)1/(sj3)):(IKReal)1.0e30))), new_r22);
                sj4array[0]=IKsin(j4array[0]);
                cj4array[0]=IKcos(j4array[0]);
                if( j4array[0] > IKPI )
                    j4array[0]-=IK2PI;
                else if( j4array[0] < -IKPI )
                    j4array[0]+=IK2PI;
                j4valid[0] = true;
                for(int ij4 = 0; ij4 < 1; ++ij4)
                {
                if( !j4valid[ij4] ) { continue; }
                j4 = j4array[ij4]; cj4 = cj4array[ij4]; sj4 = sj4array[ij4];
                
                {
                IKReal evalcond[1];
                    evalcond[0]=IKsin(j4);
                    if( IKabs(evalcond[0]) < 0.000010  )
                    {
                    {
                    IKReal j5array[1], cj5array[1], sj5array[1];
                    bool j5valid[1]={false};
                    IKReal x57=IKsin(j3);
                    IKReal x58=IKcos(j4);
                    IKReal x59=((IKabs(x58) != 0)?((IKReal)1/(x58)):(IKReal)1.0e30);
                    IKReal x60=IKcos(j3);
                    j5array[0]=IKatan2(((((-1.00000000000000)*(new_r00)*(x57)))+(((new_r10)*(x60)))), ((((new_r10)*(x57)*(x59)))+(((new_r00)*(x59)*(x60)))));
                    sj5array[0]=IKsin(j5array[0]);
                    cj5array[0]=IKcos(j5array[0]);
                    if( j5array[0] > IKPI )
                        j5array[0]-=IK2PI;
                    else if( j5array[0] < -IKPI )
                        j5array[0]+=IK2PI;
                    j5valid[0] = true;
                    for(int ij5 = 0; ij5 < 1; ++ij5)
                    {
                    if( !j5valid[ij5] ) { continue; }
                    j5 = j5array[ij5]; cj5 = cj5array[ij5]; sj5 = sj5array[ij5];
                    
                    vsolutions.push_back(IKSolution()); IKSolution& solution = vsolutions.back();
                    solution.basesol.resize(6);
                    solution.basesol[0].foffset = j0;
                    solution.basesol[1].foffset = j1;
                    solution.basesol[2].foffset = j2;
                    solution.basesol[3].foffset = j3;
                    solution.basesol[4].foffset = j4;
                    solution.basesol[5].foffset = j5;
                    solution.vfree.resize(0);
                    }
                    }
                
                } else
                {
                    if( 1 )
                    {
                    {
                    IKReal j5array[1], cj5array[1], sj5array[1];
                    bool j5valid[1]={false};
                    IKReal x61=((IKabs(sj4) != 0)?((IKReal)1/(sj4)):(IKReal)1.0e30);
                    j5array[0]=IKatan2(((new_r21)*(x61)), ((-1.00000000000000)*(new_r20)*(x61)));
                    sj5array[0]=IKsin(j5array[0]);
                    cj5array[0]=IKcos(j5array[0]);
                    if( j5array[0] > IKPI )
                        j5array[0]-=IK2PI;
                    else if( j5array[0] < -IKPI )
                        j5array[0]+=IK2PI;
                    j5valid[0] = true;
                    for(int ij5 = 0; ij5 < 1; ++ij5)
                    {
                    if( !j5valid[ij5] ) { continue; }
                    j5 = j5array[ij5]; cj5 = cj5array[ij5]; sj5 = sj5array[ij5];
                    
                    vsolutions.push_back(IKSolution()); IKSolution& solution = vsolutions.back();
                    solution.basesol.resize(6);
                    solution.basesol[0].foffset = j0;
                    solution.basesol[1].foffset = j1;
                    solution.basesol[2].foffset = j2;
                    solution.basesol[3].foffset = j3;
                    solution.basesol[4].foffset = j4;
                    solution.basesol[5].foffset = j5;
                    solution.vfree.resize(0);
                    }
                    }
                
                } else
                {
                }
                }
                }
                }
                }
            
            } else
            {
            }
            }
            }
            }
            }
        
        }
        }
    }};


/// solves the inverse kinematics equations.
/// \param pfree is an array specifying the free joints of the chain.
IKFAST_API bool ik(const IKReal* eetrans, const IKReal* eerot, const IKReal* pfree, std::vector<IKSolution>& vsolutions) {
IKSolver solver;
return solver.ik(eetrans,eerot,pfree,vsolutions);
}

IKFAST_API const char* getKinematicsHash() { return "1212e32be9160d1dd10dda70c49c46d3"; }

#ifndef IKFAST_NO_MAIN
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv)
{
    if( argc != 12+getNumFreeParameters()+1 ) {
        printf("\nUsage: ./ik r00 r01 r02 t0 r10 r11 r12 t1 r20 r21 r22 t2 free0 ...\n\n"
               "Returns the ik solutions given the transformation of the end effector specified by\n"
               "a 3x3 rotation R (rXX), and a 3x1 translation (tX).\n"
               "There are %d free parameters that have to be specified.\n\n",getNumFreeParameters());
        return 1;
    }

    std::vector<IKSolution> vsolutions;
    std::vector<IKReal> vfree(getNumFreeParameters());
    IKReal eerot[9],eetrans[3];
    eerot[0] = atof(argv[1]); eerot[1] = atof(argv[2]); eerot[2] = atof(argv[3]); eetrans[0] = atof(argv[4]);
    eerot[3] = atof(argv[5]); eerot[4] = atof(argv[6]); eerot[5] = atof(argv[7]); eetrans[1] = atof(argv[8]);
    eerot[6] = atof(argv[9]); eerot[7] = atof(argv[10]); eerot[8] = atof(argv[11]); eetrans[2] = atof(argv[12]);
    for(size_t i = 0; i < vfree.size(); ++i)
        vfree[i] = atof(argv[13+i]);
    bool bSuccess = ik(eetrans, eerot, vfree.size() > 0 ? &vfree[0] : NULL, vsolutions);

    if( !bSuccess ) {
        fprintf(stderr,"Failed to get ik solution\n");
        return -1;
    }

    printf("Found %d ik solutions:\n", (int)vsolutions.size());
    std::vector<IKReal> sol(getNumJoints());
    for(size_t i = 0; i < vsolutions.size(); ++i) {
        printf("sol%d (free=%d): ", (int)i, (int)vsolutions[i].GetFree().size());
        std::vector<IKReal> vsolfree(vsolutions[i].GetFree().size());
        vsolutions[i].GetSolution(&sol[0],vsolfree.size()>0?&vsolfree[0]:NULL);
        for( size_t j = 0; j < sol.size(); ++j)
            printf("%f, ", (float)sol[j]);
        printf("\n");
    }
    return 0;
}

#endif
