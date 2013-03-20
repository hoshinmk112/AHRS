#ifndef DCM_H
#define DCM_H

#ifdef __cplusplus
extern "C"{
#endif

extern void BuildDCM(Matrix3* Mat, float someRoll, float somePitch, float someHeadingAngle);
extern void Update(boolean rate);
extern void CorrectHeading(void);

#ifdef __cplusplus
}
#endif

#endif // DCM_H