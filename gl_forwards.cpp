#define WIN32_LEAN_AND_MEAN
#include <windows.h>

extern "C" {
    FARPROC p_GlmfBeginGlsBlock;
    FARPROC p_GlmfCloseMetaFile;
    FARPROC p_GlmfEndGlsBlock;
    FARPROC p_GlmfEndPlayback;
    FARPROC p_GlmfInitPlayback;
    FARPROC p_GlmfPlayGlsRecord;
    FARPROC p_glAccum;
    FARPROC p_glAlphaFunc;
    FARPROC p_glAreTexturesResident;
    FARPROC p_glArrayElement;
    FARPROC p_glBegin;
    FARPROC p_glBindTexture;
    FARPROC p_glBitmap;
    FARPROC p_glBlendFunc;
    FARPROC p_glCallList;
    FARPROC p_glCallLists;
    FARPROC p_glClearAccum;
    FARPROC p_glClearColor;
    FARPROC p_glClearDepth;
    FARPROC p_glClearIndex;
    FARPROC p_glClearStencil;
    FARPROC p_glClipPlane;
    FARPROC p_glColor3b;
    FARPROC p_glColor3bv;
    FARPROC p_glColor3d;
    FARPROC p_glColor3dv;
    FARPROC p_glColor3f;
    FARPROC p_glColor3fv;
    FARPROC p_glColor3i;
    FARPROC p_glColor3iv;
    FARPROC p_glColor3s;
    FARPROC p_glColor3sv;
    FARPROC p_glColor3ub;
    FARPROC p_glColor3ubv;
    FARPROC p_glColor3ui;
    FARPROC p_glColor3uiv;
    FARPROC p_glColor3us;
    FARPROC p_glColor3usv;
    FARPROC p_glColor4b;
    FARPROC p_glColor4bv;
    FARPROC p_glColor4d;
    FARPROC p_glColor4dv;
    FARPROC p_glColor4f;
    FARPROC p_glColor4fv;
    FARPROC p_glColor4i;
    FARPROC p_glColor4iv;
    FARPROC p_glColor4s;
    FARPROC p_glColor4sv;
    FARPROC p_glColor4ub;
    FARPROC p_glColor4ubv;
    FARPROC p_glColor4ui;
    FARPROC p_glColor4uiv;
    FARPROC p_glColor4us;
    FARPROC p_glColor4usv;
    FARPROC p_glColorMask;
    FARPROC p_glColorMaterial;
    FARPROC p_glColorPointer;
    FARPROC p_glCopyPixels;
    FARPROC p_glCopyTexImage1D;
    FARPROC p_glCopyTexImage2D;
    FARPROC p_glCopyTexSubImage1D;
    FARPROC p_glCopyTexSubImage2D;
    FARPROC p_glCullFace;
    FARPROC p_glDebugEntry;
    FARPROC p_glDeleteLists;
    FARPROC p_glDeleteTextures;
    FARPROC p_glDepthFunc;
    FARPROC p_glDepthMask;
    FARPROC p_glDepthRange;
    FARPROC p_glDisable;
    FARPROC p_glDisableClientState;
    FARPROC p_glDrawArrays;
    FARPROC p_glDrawBuffer;
    FARPROC p_glDrawElements;
    FARPROC p_glDrawPixels;
    FARPROC p_glEdgeFlag;
    FARPROC p_glEdgeFlagPointer;
    FARPROC p_glEdgeFlagv;
    FARPROC p_glEnable;
    FARPROC p_glEnableClientState;
    FARPROC p_glEnd;
    FARPROC p_glEndList;
    FARPROC p_glEvalCoord1d;
    FARPROC p_glEvalCoord1dv;
    FARPROC p_glEvalCoord1f;
    FARPROC p_glEvalCoord1fv;
    FARPROC p_glEvalCoord2d;
    FARPROC p_glEvalCoord2dv;
    FARPROC p_glEvalCoord2f;
    FARPROC p_glEvalCoord2fv;
    FARPROC p_glEvalMesh1;
    FARPROC p_glEvalMesh2;
    FARPROC p_glEvalPoint1;
    FARPROC p_glEvalPoint2;
    FARPROC p_glFeedbackBuffer;
    FARPROC p_glFinish;
    FARPROC p_glFlush;
    FARPROC p_glFogf;
    FARPROC p_glFogfv;
    FARPROC p_glFogi;
    FARPROC p_glFogiv;
    FARPROC p_glFrontFace;
    FARPROC p_glFrustum;
    FARPROC p_glGenLists;
    FARPROC p_glGenTextures;
    FARPROC p_glGetBooleanv;
    FARPROC p_glGetClipPlane;
    FARPROC p_glGetDoublev;
    FARPROC p_glGetError;
    FARPROC p_glGetFloatv;
    FARPROC p_glGetLightfv;
    FARPROC p_glGetLightiv;
    FARPROC p_glGetMapdv;
    FARPROC p_glGetMapfv;
    FARPROC p_glGetMapiv;
    FARPROC p_glGetMaterialfv;
    FARPROC p_glGetMaterialiv;
    FARPROC p_glGetPixelMapfv;
    FARPROC p_glGetPixelMapuiv;
    FARPROC p_glGetPixelMapusv;
    FARPROC p_glGetPointerv;
    FARPROC p_glGetPolygonStipple;
    FARPROC p_glGetString;
    FARPROC p_glGetTexEnvfv;
    FARPROC p_glGetTexEnviv;
    FARPROC p_glGetTexGendv;
    FARPROC p_glGetTexGenfv;
    FARPROC p_glGetTexGeniv;
    FARPROC p_glGetTexImage;
    FARPROC p_glGetTexLevelParameterfv;
    FARPROC p_glGetTexLevelParameteriv;
    FARPROC p_glGetTexParameterfv;
    FARPROC p_glGetTexParameteriv;
    FARPROC p_glHint;
    FARPROC p_glIndexMask;
    FARPROC p_glIndexPointer;
    FARPROC p_glIndexd;
    FARPROC p_glIndexdv;
    FARPROC p_glIndexf;
    FARPROC p_glIndexfv;
    FARPROC p_glIndexi;
    FARPROC p_glIndexiv;
    FARPROC p_glIndexs;
    FARPROC p_glIndexsv;
    FARPROC p_glIndexub;
    FARPROC p_glIndexubv;
    FARPROC p_glInitNames;
    FARPROC p_glInterleavedArrays;
    FARPROC p_glIsEnabled;
    FARPROC p_glIsList;
    FARPROC p_glIsTexture;
    FARPROC p_glLightModelf;
    FARPROC p_glLightModelfv;
    FARPROC p_glLightModeli;
    FARPROC p_glLightModeliv;
    FARPROC p_glLightf;
    FARPROC p_glLightfv;
    FARPROC p_glLighti;
    FARPROC p_glLightiv;
    FARPROC p_glLineStipple;
    FARPROC p_glLineWidth;
    FARPROC p_glListBase;
    FARPROC p_glLoadIdentity;
    FARPROC p_glLoadMatrixd;
    FARPROC p_glLoadMatrixf;
    FARPROC p_glLoadName;
    FARPROC p_glLogicOp;
    FARPROC p_glMap1d;
    FARPROC p_glMap1f;
    FARPROC p_glMap2d;
    FARPROC p_glMap2f;
    FARPROC p_glMapGrid1d;
    FARPROC p_glMapGrid1f;
    FARPROC p_glMapGrid2d;
    FARPROC p_glMapGrid2f;
    FARPROC p_glMaterialf;
    FARPROC p_glMaterialfv;
    FARPROC p_glMateriali;
    FARPROC p_glMaterialiv;
    FARPROC p_glMatrixMode;
    FARPROC p_glMultMatrixd;
    FARPROC p_glMultMatrixf;
    FARPROC p_glNewList;
    FARPROC p_glNormal3b;
    FARPROC p_glNormal3bv;
    FARPROC p_glNormal3d;
    FARPROC p_glNormal3dv;
    FARPROC p_glNormal3f;
    FARPROC p_glNormal3fv;
    FARPROC p_glNormal3i;
    FARPROC p_glNormal3iv;
    FARPROC p_glNormal3s;
    FARPROC p_glNormal3sv;
    FARPROC p_glNormalPointer;
    FARPROC p_glPassThrough;
    FARPROC p_glPixelMapfv;
    FARPROC p_glPixelMapuiv;
    FARPROC p_glPixelMapusv;
    FARPROC p_glPixelStoref;
    FARPROC p_glPixelStorei;
    FARPROC p_glPixelTransferf;
    FARPROC p_glPixelTransferi;
    FARPROC p_glPixelZoom;
    FARPROC p_glPointSize;
    FARPROC p_glPolygonMode;
    FARPROC p_glPolygonOffset;
    FARPROC p_glPolygonStipple;
    FARPROC p_glPopAttrib;
    FARPROC p_glPopClientAttrib;
    FARPROC p_glPopMatrix;
    FARPROC p_glPopName;
    FARPROC p_glPrioritizeTextures;
    FARPROC p_glPushAttrib;
    FARPROC p_glPushClientAttrib;
    FARPROC p_glPushMatrix;
    FARPROC p_glPushName;
    FARPROC p_glRasterPos2d;
    FARPROC p_glRasterPos2dv;
    FARPROC p_glRasterPos2f;
    FARPROC p_glRasterPos2fv;
    FARPROC p_glRasterPos2i;
    FARPROC p_glRasterPos2iv;
    FARPROC p_glRasterPos2s;
    FARPROC p_glRasterPos2sv;
    FARPROC p_glRasterPos3d;
    FARPROC p_glRasterPos3dv;
    FARPROC p_glRasterPos3f;
    FARPROC p_glRasterPos3fv;
    FARPROC p_glRasterPos3i;
    FARPROC p_glRasterPos3iv;
    FARPROC p_glRasterPos3s;
    FARPROC p_glRasterPos3sv;
    FARPROC p_glRasterPos4d;
    FARPROC p_glRasterPos4dv;
    FARPROC p_glRasterPos4f;
    FARPROC p_glRasterPos4fv;
    FARPROC p_glRasterPos4i;
    FARPROC p_glRasterPos4iv;
    FARPROC p_glRasterPos4s;
    FARPROC p_glRasterPos4sv;
    FARPROC p_glReadBuffer;
    FARPROC p_glReadPixels;
    FARPROC p_glRectd;
    FARPROC p_glRectdv;
    FARPROC p_glRectf;
    FARPROC p_glRectfv;
    FARPROC p_glRecti;
    FARPROC p_glRectiv;
    FARPROC p_glRects;
    FARPROC p_glRectsv;
    FARPROC p_glRenderMode;
    FARPROC p_glRotated;
    FARPROC p_glRotatef;
    FARPROC p_glScaled;
    FARPROC p_glScalef;
    FARPROC p_glSelectBuffer;
    FARPROC p_glShadeModel;
    FARPROC p_glStencilFunc;
    FARPROC p_glStencilMask;
    FARPROC p_glStencilOp;
    FARPROC p_glTexCoord1d;
    FARPROC p_glTexCoord1dv;
    FARPROC p_glTexCoord1f;
    FARPROC p_glTexCoord1fv;
    FARPROC p_glTexCoord1i;
    FARPROC p_glTexCoord1iv;
    FARPROC p_glTexCoord1s;
    FARPROC p_glTexCoord1sv;
    FARPROC p_glTexCoord2d;
    FARPROC p_glTexCoord2dv;
    FARPROC p_glTexCoord2f;
    FARPROC p_glTexCoord2fv;
    FARPROC p_glTexCoord2i;
    FARPROC p_glTexCoord2iv;
    FARPROC p_glTexCoord2s;
    FARPROC p_glTexCoord2sv;
    FARPROC p_glTexCoord3d;
    FARPROC p_glTexCoord3dv;
    FARPROC p_glTexCoord3f;
    FARPROC p_glTexCoord3fv;
    FARPROC p_glTexCoord3i;
    FARPROC p_glTexCoord3iv;
    FARPROC p_glTexCoord3s;
    FARPROC p_glTexCoord3sv;
    FARPROC p_glTexCoord4d;
    FARPROC p_glTexCoord4dv;
    FARPROC p_glTexCoord4f;
    FARPROC p_glTexCoord4fv;
    FARPROC p_glTexCoord4i;
    FARPROC p_glTexCoord4iv;
    FARPROC p_glTexCoord4s;
    FARPROC p_glTexCoord4sv;
    FARPROC p_glTexCoordPointer;
    FARPROC p_glTexEnvf;
    FARPROC p_glTexEnvfv;
    FARPROC p_glTexEnvi;
    FARPROC p_glTexEnviv;
    FARPROC p_glTexGend;
    FARPROC p_glTexGendv;
    FARPROC p_glTexGenf;
    FARPROC p_glTexGenfv;
    FARPROC p_glTexGeni;
    FARPROC p_glTexGeniv;
    FARPROC p_glTexImage1D;
    FARPROC p_glTexImage2D;
    FARPROC p_glTexParameterf;
    FARPROC p_glTexParameterfv;
    FARPROC p_glTexParameteri;
    FARPROC p_glTexParameteriv;
    FARPROC p_glTexSubImage1D;
    FARPROC p_glTexSubImage2D;
    FARPROC p_glTranslated;
    FARPROC p_glTranslatef;
    FARPROC p_glVertex2d;
    FARPROC p_glVertex2dv;
    FARPROC p_glVertex2f;
    FARPROC p_glVertex2fv;
    FARPROC p_glVertex2i;
    FARPROC p_glVertex2iv;
    FARPROC p_glVertex2s;
    FARPROC p_glVertex2sv;
    FARPROC p_glVertex3d;
    FARPROC p_glVertex3dv;
    FARPROC p_glVertex3f;
    FARPROC p_glVertex3fv;
    FARPROC p_glVertex3i;
    FARPROC p_glVertex3iv;
    FARPROC p_glVertex3s;
    FARPROC p_glVertex3sv;
    FARPROC p_glVertex4d;
    FARPROC p_glVertex4dv;
    FARPROC p_glVertex4f;
    FARPROC p_glVertex4fv;
    FARPROC p_glVertex4i;
    FARPROC p_glVertex4iv;
    FARPROC p_glVertex4s;
    FARPROC p_glVertex4sv;
    FARPROC p_glVertexPointer;
    FARPROC p_wglChoosePixelFormat;
    FARPROC p_wglCopyContext;
    FARPROC p_wglCreateContext;
    FARPROC p_wglCreateLayerContext;
    FARPROC p_wglDeleteContext;
    FARPROC p_wglDescribeLayerPlane;
    FARPROC p_wglDescribePixelFormat;
    FARPROC p_wglGetCurrentContext;
    FARPROC p_wglGetCurrentDC;
    FARPROC p_wglGetDefaultProcAddress;
    FARPROC p_wglGetLayerPaletteEntries;
    FARPROC p_wglGetPixelFormat;
    FARPROC p_wglRealizeLayerPalette;
    FARPROC p_wglSetLayerPaletteEntries;
    FARPROC p_wglSetPixelFormat;
    FARPROC p_wglShareLists;
    FARPROC p_wglSwapBuffers;
    FARPROC p_wglSwapLayerBuffers;
    FARPROC p_wglSwapMultipleBuffers;
    FARPROC p_wglUseFontBitmapsA;
    FARPROC p_wglUseFontBitmapsW;
    FARPROC p_wglUseFontOutlinesA;
    FARPROC p_wglUseFontOutlinesW;
}

extern "C" __declspec(naked) void tramp_GlmfBeginGlsBlock() { __asm { jmp dword ptr [p_GlmfBeginGlsBlock] } }
extern "C" __declspec(naked) void tramp_GlmfCloseMetaFile() { __asm { jmp dword ptr [p_GlmfCloseMetaFile] } }
extern "C" __declspec(naked) void tramp_GlmfEndGlsBlock() { __asm { jmp dword ptr [p_GlmfEndGlsBlock] } }
extern "C" __declspec(naked) void tramp_GlmfEndPlayback() { __asm { jmp dword ptr [p_GlmfEndPlayback] } }
extern "C" __declspec(naked) void tramp_GlmfInitPlayback() { __asm { jmp dword ptr [p_GlmfInitPlayback] } }
extern "C" __declspec(naked) void tramp_GlmfPlayGlsRecord() { __asm { jmp dword ptr [p_GlmfPlayGlsRecord] } }
extern "C" __declspec(naked) void tramp_glAccum() { __asm { jmp dword ptr [p_glAccum] } }
extern "C" __declspec(naked) void tramp_glAlphaFunc() { __asm { jmp dword ptr [p_glAlphaFunc] } }
extern "C" __declspec(naked) void tramp_glAreTexturesResident() { __asm { jmp dword ptr [p_glAreTexturesResident] } }
extern "C" __declspec(naked) void tramp_glArrayElement() { __asm { jmp dword ptr [p_glArrayElement] } }
extern "C" __declspec(naked) void tramp_glBegin() { __asm { jmp dword ptr [p_glBegin] } }
extern "C" __declspec(naked) void tramp_glBindTexture() { __asm { jmp dword ptr [p_glBindTexture] } }
extern "C" __declspec(naked) void tramp_glBitmap() { __asm { jmp dword ptr [p_glBitmap] } }
extern "C" __declspec(naked) void tramp_glBlendFunc() { __asm { jmp dword ptr [p_glBlendFunc] } }
extern "C" __declspec(naked) void tramp_glCallList() { __asm { jmp dword ptr [p_glCallList] } }
extern "C" __declspec(naked) void tramp_glCallLists() { __asm { jmp dword ptr [p_glCallLists] } }
extern "C" __declspec(naked) void tramp_glClearAccum() { __asm { jmp dword ptr [p_glClearAccum] } }
extern "C" __declspec(naked) void tramp_glClearColor() { __asm { jmp dword ptr [p_glClearColor] } }
extern "C" __declspec(naked) void tramp_glClearDepth() { __asm { jmp dword ptr [p_glClearDepth] } }
extern "C" __declspec(naked) void tramp_glClearIndex() { __asm { jmp dword ptr [p_glClearIndex] } }
extern "C" __declspec(naked) void tramp_glClearStencil() { __asm { jmp dword ptr [p_glClearStencil] } }
extern "C" __declspec(naked) void tramp_glClipPlane() { __asm { jmp dword ptr [p_glClipPlane] } }
extern "C" __declspec(naked) void tramp_glColor3b() { __asm { jmp dword ptr [p_glColor3b] } }
extern "C" __declspec(naked) void tramp_glColor3bv() { __asm { jmp dword ptr [p_glColor3bv] } }
extern "C" __declspec(naked) void tramp_glColor3d() { __asm { jmp dword ptr [p_glColor3d] } }
extern "C" __declspec(naked) void tramp_glColor3dv() { __asm { jmp dword ptr [p_glColor3dv] } }
extern "C" __declspec(naked) void tramp_glColor3f() { __asm { jmp dword ptr [p_glColor3f] } }
extern "C" __declspec(naked) void tramp_glColor3fv() { __asm { jmp dword ptr [p_glColor3fv] } }
extern "C" __declspec(naked) void tramp_glColor3i() { __asm { jmp dword ptr [p_glColor3i] } }
extern "C" __declspec(naked) void tramp_glColor3iv() { __asm { jmp dword ptr [p_glColor3iv] } }
extern "C" __declspec(naked) void tramp_glColor3s() { __asm { jmp dword ptr [p_glColor3s] } }
extern "C" __declspec(naked) void tramp_glColor3sv() { __asm { jmp dword ptr [p_glColor3sv] } }
extern "C" __declspec(naked) void tramp_glColor3ub() { __asm { jmp dword ptr [p_glColor3ub] } }
extern "C" __declspec(naked) void tramp_glColor3ubv() { __asm { jmp dword ptr [p_glColor3ubv] } }
extern "C" __declspec(naked) void tramp_glColor3ui() { __asm { jmp dword ptr [p_glColor3ui] } }
extern "C" __declspec(naked) void tramp_glColor3uiv() { __asm { jmp dword ptr [p_glColor3uiv] } }
extern "C" __declspec(naked) void tramp_glColor3us() { __asm { jmp dword ptr [p_glColor3us] } }
extern "C" __declspec(naked) void tramp_glColor3usv() { __asm { jmp dword ptr [p_glColor3usv] } }
extern "C" __declspec(naked) void tramp_glColor4b() { __asm { jmp dword ptr [p_glColor4b] } }
extern "C" __declspec(naked) void tramp_glColor4bv() { __asm { jmp dword ptr [p_glColor4bv] } }
extern "C" __declspec(naked) void tramp_glColor4d() { __asm { jmp dword ptr [p_glColor4d] } }
extern "C" __declspec(naked) void tramp_glColor4dv() { __asm { jmp dword ptr [p_glColor4dv] } }
extern "C" __declspec(naked) void tramp_glColor4f() { __asm { jmp dword ptr [p_glColor4f] } }
extern "C" __declspec(naked) void tramp_glColor4fv() { __asm { jmp dword ptr [p_glColor4fv] } }
extern "C" __declspec(naked) void tramp_glColor4i() { __asm { jmp dword ptr [p_glColor4i] } }
extern "C" __declspec(naked) void tramp_glColor4iv() { __asm { jmp dword ptr [p_glColor4iv] } }
extern "C" __declspec(naked) void tramp_glColor4s() { __asm { jmp dword ptr [p_glColor4s] } }
extern "C" __declspec(naked) void tramp_glColor4sv() { __asm { jmp dword ptr [p_glColor4sv] } }
extern "C" __declspec(naked) void tramp_glColor4ub() { __asm { jmp dword ptr [p_glColor4ub] } }
extern "C" __declspec(naked) void tramp_glColor4ubv() { __asm { jmp dword ptr [p_glColor4ubv] } }
extern "C" __declspec(naked) void tramp_glColor4ui() { __asm { jmp dword ptr [p_glColor4ui] } }
extern "C" __declspec(naked) void tramp_glColor4uiv() { __asm { jmp dword ptr [p_glColor4uiv] } }
extern "C" __declspec(naked) void tramp_glColor4us() { __asm { jmp dword ptr [p_glColor4us] } }
extern "C" __declspec(naked) void tramp_glColor4usv() { __asm { jmp dword ptr [p_glColor4usv] } }
extern "C" __declspec(naked) void tramp_glColorMask() { __asm { jmp dword ptr [p_glColorMask] } }
extern "C" __declspec(naked) void tramp_glColorMaterial() { __asm { jmp dword ptr [p_glColorMaterial] } }
extern "C" __declspec(naked) void tramp_glColorPointer() { __asm { jmp dword ptr [p_glColorPointer] } }
extern "C" __declspec(naked) void tramp_glCopyPixels() { __asm { jmp dword ptr [p_glCopyPixels] } }
extern "C" __declspec(naked) void tramp_glCopyTexImage1D() { __asm { jmp dword ptr [p_glCopyTexImage1D] } }
extern "C" __declspec(naked) void tramp_glCopyTexImage2D() { __asm { jmp dword ptr [p_glCopyTexImage2D] } }
extern "C" __declspec(naked) void tramp_glCopyTexSubImage1D() { __asm { jmp dword ptr [p_glCopyTexSubImage1D] } }
extern "C" __declspec(naked) void tramp_glCopyTexSubImage2D() { __asm { jmp dword ptr [p_glCopyTexSubImage2D] } }
extern "C" __declspec(naked) void tramp_glCullFace() { __asm { jmp dword ptr [p_glCullFace] } }
extern "C" __declspec(naked) void tramp_glDebugEntry() { __asm { jmp dword ptr [p_glDebugEntry] } }
extern "C" __declspec(naked) void tramp_glDeleteLists() { __asm { jmp dword ptr [p_glDeleteLists] } }
extern "C" __declspec(naked) void tramp_glDeleteTextures() { __asm { jmp dword ptr [p_glDeleteTextures] } }
extern "C" __declspec(naked) void tramp_glDepthFunc() { __asm { jmp dword ptr [p_glDepthFunc] } }
extern "C" __declspec(naked) void tramp_glDepthMask() { __asm { jmp dword ptr [p_glDepthMask] } }
extern "C" __declspec(naked) void tramp_glDepthRange() { __asm { jmp dword ptr [p_glDepthRange] } }
extern "C" __declspec(naked) void tramp_glDisable() { __asm { jmp dword ptr [p_glDisable] } }
extern "C" __declspec(naked) void tramp_glDisableClientState() { __asm { jmp dword ptr [p_glDisableClientState] } }
extern "C" __declspec(naked) void tramp_glDrawArrays() { __asm { jmp dword ptr [p_glDrawArrays] } }
extern "C" __declspec(naked) void tramp_glDrawBuffer() { __asm { jmp dword ptr [p_glDrawBuffer] } }
extern "C" __declspec(naked) void tramp_glDrawElements() { __asm { jmp dword ptr [p_glDrawElements] } }
extern "C" __declspec(naked) void tramp_glDrawPixels() { __asm { jmp dword ptr [p_glDrawPixels] } }
extern "C" __declspec(naked) void tramp_glEdgeFlag() { __asm { jmp dword ptr [p_glEdgeFlag] } }
extern "C" __declspec(naked) void tramp_glEdgeFlagPointer() { __asm { jmp dword ptr [p_glEdgeFlagPointer] } }
extern "C" __declspec(naked) void tramp_glEdgeFlagv() { __asm { jmp dword ptr [p_glEdgeFlagv] } }
extern "C" __declspec(naked) void tramp_glEnable() { __asm { jmp dword ptr [p_glEnable] } }
extern "C" __declspec(naked) void tramp_glEnableClientState() { __asm { jmp dword ptr [p_glEnableClientState] } }
extern "C" __declspec(naked) void tramp_glEnd() { __asm { jmp dword ptr [p_glEnd] } }
extern "C" __declspec(naked) void tramp_glEndList() { __asm { jmp dword ptr [p_glEndList] } }
extern "C" __declspec(naked) void tramp_glEvalCoord1d() { __asm { jmp dword ptr [p_glEvalCoord1d] } }
extern "C" __declspec(naked) void tramp_glEvalCoord1dv() { __asm { jmp dword ptr [p_glEvalCoord1dv] } }
extern "C" __declspec(naked) void tramp_glEvalCoord1f() { __asm { jmp dword ptr [p_glEvalCoord1f] } }
extern "C" __declspec(naked) void tramp_glEvalCoord1fv() { __asm { jmp dword ptr [p_glEvalCoord1fv] } }
extern "C" __declspec(naked) void tramp_glEvalCoord2d() { __asm { jmp dword ptr [p_glEvalCoord2d] } }
extern "C" __declspec(naked) void tramp_glEvalCoord2dv() { __asm { jmp dword ptr [p_glEvalCoord2dv] } }
extern "C" __declspec(naked) void tramp_glEvalCoord2f() { __asm { jmp dword ptr [p_glEvalCoord2f] } }
extern "C" __declspec(naked) void tramp_glEvalCoord2fv() { __asm { jmp dword ptr [p_glEvalCoord2fv] } }
extern "C" __declspec(naked) void tramp_glEvalMesh1() { __asm { jmp dword ptr [p_glEvalMesh1] } }
extern "C" __declspec(naked) void tramp_glEvalMesh2() { __asm { jmp dword ptr [p_glEvalMesh2] } }
extern "C" __declspec(naked) void tramp_glEvalPoint1() { __asm { jmp dword ptr [p_glEvalPoint1] } }
extern "C" __declspec(naked) void tramp_glEvalPoint2() { __asm { jmp dword ptr [p_glEvalPoint2] } }
extern "C" __declspec(naked) void tramp_glFeedbackBuffer() { __asm { jmp dword ptr [p_glFeedbackBuffer] } }
extern "C" __declspec(naked) void tramp_glFinish() { __asm { jmp dword ptr [p_glFinish] } }
extern "C" __declspec(naked) void tramp_glFlush() { __asm { jmp dword ptr [p_glFlush] } }
extern "C" __declspec(naked) void tramp_glFogf() { __asm { jmp dword ptr [p_glFogf] } }
extern "C" __declspec(naked) void tramp_glFogfv() { __asm { jmp dword ptr [p_glFogfv] } }
extern "C" __declspec(naked) void tramp_glFogi() { __asm { jmp dword ptr [p_glFogi] } }
extern "C" __declspec(naked) void tramp_glFogiv() { __asm { jmp dword ptr [p_glFogiv] } }
extern "C" __declspec(naked) void tramp_glFrontFace() { __asm { jmp dword ptr [p_glFrontFace] } }
extern "C" __declspec(naked) void tramp_glFrustum() { __asm { jmp dword ptr [p_glFrustum] } }
extern "C" __declspec(naked) void tramp_glGenLists() { __asm { jmp dword ptr [p_glGenLists] } }
extern "C" __declspec(naked) void tramp_glGenTextures() { __asm { jmp dword ptr [p_glGenTextures] } }
extern "C" __declspec(naked) void tramp_glGetBooleanv() { __asm { jmp dword ptr [p_glGetBooleanv] } }
extern "C" __declspec(naked) void tramp_glGetClipPlane() { __asm { jmp dword ptr [p_glGetClipPlane] } }
extern "C" __declspec(naked) void tramp_glGetDoublev() { __asm { jmp dword ptr [p_glGetDoublev] } }
extern "C" __declspec(naked) void tramp_glGetError() { __asm { jmp dword ptr [p_glGetError] } }
extern "C" __declspec(naked) void tramp_glGetFloatv() { __asm { jmp dword ptr [p_glGetFloatv] } }
extern "C" __declspec(naked) void tramp_glGetLightfv() { __asm { jmp dword ptr [p_glGetLightfv] } }
extern "C" __declspec(naked) void tramp_glGetLightiv() { __asm { jmp dword ptr [p_glGetLightiv] } }
extern "C" __declspec(naked) void tramp_glGetMapdv() { __asm { jmp dword ptr [p_glGetMapdv] } }
extern "C" __declspec(naked) void tramp_glGetMapfv() { __asm { jmp dword ptr [p_glGetMapfv] } }
extern "C" __declspec(naked) void tramp_glGetMapiv() { __asm { jmp dword ptr [p_glGetMapiv] } }
extern "C" __declspec(naked) void tramp_glGetMaterialfv() { __asm { jmp dword ptr [p_glGetMaterialfv] } }
extern "C" __declspec(naked) void tramp_glGetMaterialiv() { __asm { jmp dword ptr [p_glGetMaterialiv] } }
extern "C" __declspec(naked) void tramp_glGetPixelMapfv() { __asm { jmp dword ptr [p_glGetPixelMapfv] } }
extern "C" __declspec(naked) void tramp_glGetPixelMapuiv() { __asm { jmp dword ptr [p_glGetPixelMapuiv] } }
extern "C" __declspec(naked) void tramp_glGetPixelMapusv() { __asm { jmp dword ptr [p_glGetPixelMapusv] } }
extern "C" __declspec(naked) void tramp_glGetPointerv() { __asm { jmp dword ptr [p_glGetPointerv] } }
extern "C" __declspec(naked) void tramp_glGetPolygonStipple() { __asm { jmp dword ptr [p_glGetPolygonStipple] } }
extern "C" __declspec(naked) void tramp_glGetString() { __asm { jmp dword ptr [p_glGetString] } }
extern "C" __declspec(naked) void tramp_glGetTexEnvfv() { __asm { jmp dword ptr [p_glGetTexEnvfv] } }
extern "C" __declspec(naked) void tramp_glGetTexEnviv() { __asm { jmp dword ptr [p_glGetTexEnviv] } }
extern "C" __declspec(naked) void tramp_glGetTexGendv() { __asm { jmp dword ptr [p_glGetTexGendv] } }
extern "C" __declspec(naked) void tramp_glGetTexGenfv() { __asm { jmp dword ptr [p_glGetTexGenfv] } }
extern "C" __declspec(naked) void tramp_glGetTexGeniv() { __asm { jmp dword ptr [p_glGetTexGeniv] } }
extern "C" __declspec(naked) void tramp_glGetTexImage() { __asm { jmp dword ptr [p_glGetTexImage] } }
extern "C" __declspec(naked) void tramp_glGetTexLevelParameterfv() { __asm { jmp dword ptr [p_glGetTexLevelParameterfv] } }
extern "C" __declspec(naked) void tramp_glGetTexLevelParameteriv() { __asm { jmp dword ptr [p_glGetTexLevelParameteriv] } }
extern "C" __declspec(naked) void tramp_glGetTexParameterfv() { __asm { jmp dword ptr [p_glGetTexParameterfv] } }
extern "C" __declspec(naked) void tramp_glGetTexParameteriv() { __asm { jmp dword ptr [p_glGetTexParameteriv] } }
extern "C" __declspec(naked) void tramp_glHint() { __asm { jmp dword ptr [p_glHint] } }
extern "C" __declspec(naked) void tramp_glIndexMask() { __asm { jmp dword ptr [p_glIndexMask] } }
extern "C" __declspec(naked) void tramp_glIndexPointer() { __asm { jmp dword ptr [p_glIndexPointer] } }
extern "C" __declspec(naked) void tramp_glIndexd() { __asm { jmp dword ptr [p_glIndexd] } }
extern "C" __declspec(naked) void tramp_glIndexdv() { __asm { jmp dword ptr [p_glIndexdv] } }
extern "C" __declspec(naked) void tramp_glIndexf() { __asm { jmp dword ptr [p_glIndexf] } }
extern "C" __declspec(naked) void tramp_glIndexfv() { __asm { jmp dword ptr [p_glIndexfv] } }
extern "C" __declspec(naked) void tramp_glIndexi() { __asm { jmp dword ptr [p_glIndexi] } }
extern "C" __declspec(naked) void tramp_glIndexiv() { __asm { jmp dword ptr [p_glIndexiv] } }
extern "C" __declspec(naked) void tramp_glIndexs() { __asm { jmp dword ptr [p_glIndexs] } }
extern "C" __declspec(naked) void tramp_glIndexsv() { __asm { jmp dword ptr [p_glIndexsv] } }
extern "C" __declspec(naked) void tramp_glIndexub() { __asm { jmp dword ptr [p_glIndexub] } }
extern "C" __declspec(naked) void tramp_glIndexubv() { __asm { jmp dword ptr [p_glIndexubv] } }
extern "C" __declspec(naked) void tramp_glInitNames() { __asm { jmp dword ptr [p_glInitNames] } }
extern "C" __declspec(naked) void tramp_glInterleavedArrays() { __asm { jmp dword ptr [p_glInterleavedArrays] } }
extern "C" __declspec(naked) void tramp_glIsEnabled() { __asm { jmp dword ptr [p_glIsEnabled] } }
extern "C" __declspec(naked) void tramp_glIsList() { __asm { jmp dword ptr [p_glIsList] } }
extern "C" __declspec(naked) void tramp_glIsTexture() { __asm { jmp dword ptr [p_glIsTexture] } }
extern "C" __declspec(naked) void tramp_glLightModelf() { __asm { jmp dword ptr [p_glLightModelf] } }
extern "C" __declspec(naked) void tramp_glLightModelfv() { __asm { jmp dword ptr [p_glLightModelfv] } }
extern "C" __declspec(naked) void tramp_glLightModeli() { __asm { jmp dword ptr [p_glLightModeli] } }
extern "C" __declspec(naked) void tramp_glLightModeliv() { __asm { jmp dword ptr [p_glLightModeliv] } }
extern "C" __declspec(naked) void tramp_glLightf() { __asm { jmp dword ptr [p_glLightf] } }
extern "C" __declspec(naked) void tramp_glLightfv() { __asm { jmp dword ptr [p_glLightfv] } }
extern "C" __declspec(naked) void tramp_glLighti() { __asm { jmp dword ptr [p_glLighti] } }
extern "C" __declspec(naked) void tramp_glLightiv() { __asm { jmp dword ptr [p_glLightiv] } }
extern "C" __declspec(naked) void tramp_glLineStipple() { __asm { jmp dword ptr [p_glLineStipple] } }
extern "C" __declspec(naked) void tramp_glLineWidth() { __asm { jmp dword ptr [p_glLineWidth] } }
extern "C" __declspec(naked) void tramp_glListBase() { __asm { jmp dword ptr [p_glListBase] } }
extern "C" __declspec(naked) void tramp_glLoadIdentity() { __asm { jmp dword ptr [p_glLoadIdentity] } }
extern "C" __declspec(naked) void tramp_glLoadMatrixd() { __asm { jmp dword ptr [p_glLoadMatrixd] } }
extern "C" __declspec(naked) void tramp_glLoadMatrixf() { __asm { jmp dword ptr [p_glLoadMatrixf] } }
extern "C" __declspec(naked) void tramp_glLoadName() { __asm { jmp dword ptr [p_glLoadName] } }
extern "C" __declspec(naked) void tramp_glLogicOp() { __asm { jmp dword ptr [p_glLogicOp] } }
extern "C" __declspec(naked) void tramp_glMap1d() { __asm { jmp dword ptr [p_glMap1d] } }
extern "C" __declspec(naked) void tramp_glMap1f() { __asm { jmp dword ptr [p_glMap1f] } }
extern "C" __declspec(naked) void tramp_glMap2d() { __asm { jmp dword ptr [p_glMap2d] } }
extern "C" __declspec(naked) void tramp_glMap2f() { __asm { jmp dword ptr [p_glMap2f] } }
extern "C" __declspec(naked) void tramp_glMapGrid1d() { __asm { jmp dword ptr [p_glMapGrid1d] } }
extern "C" __declspec(naked) void tramp_glMapGrid1f() { __asm { jmp dword ptr [p_glMapGrid1f] } }
extern "C" __declspec(naked) void tramp_glMapGrid2d() { __asm { jmp dword ptr [p_glMapGrid2d] } }
extern "C" __declspec(naked) void tramp_glMapGrid2f() { __asm { jmp dword ptr [p_glMapGrid2f] } }
extern "C" __declspec(naked) void tramp_glMaterialf() { __asm { jmp dword ptr [p_glMaterialf] } }
extern "C" __declspec(naked) void tramp_glMaterialfv() { __asm { jmp dword ptr [p_glMaterialfv] } }
extern "C" __declspec(naked) void tramp_glMateriali() { __asm { jmp dword ptr [p_glMateriali] } }
extern "C" __declspec(naked) void tramp_glMaterialiv() { __asm { jmp dword ptr [p_glMaterialiv] } }
extern "C" __declspec(naked) void tramp_glMatrixMode() { __asm { jmp dword ptr [p_glMatrixMode] } }
extern "C" __declspec(naked) void tramp_glMultMatrixd() { __asm { jmp dword ptr [p_glMultMatrixd] } }
extern "C" __declspec(naked) void tramp_glMultMatrixf() { __asm { jmp dword ptr [p_glMultMatrixf] } }
extern "C" __declspec(naked) void tramp_glNewList() { __asm { jmp dword ptr [p_glNewList] } }
extern "C" __declspec(naked) void tramp_glNormal3b() { __asm { jmp dword ptr [p_glNormal3b] } }
extern "C" __declspec(naked) void tramp_glNormal3bv() { __asm { jmp dword ptr [p_glNormal3bv] } }
extern "C" __declspec(naked) void tramp_glNormal3d() { __asm { jmp dword ptr [p_glNormal3d] } }
extern "C" __declspec(naked) void tramp_glNormal3dv() { __asm { jmp dword ptr [p_glNormal3dv] } }
extern "C" __declspec(naked) void tramp_glNormal3f() { __asm { jmp dword ptr [p_glNormal3f] } }
extern "C" __declspec(naked) void tramp_glNormal3fv() { __asm { jmp dword ptr [p_glNormal3fv] } }
extern "C" __declspec(naked) void tramp_glNormal3i() { __asm { jmp dword ptr [p_glNormal3i] } }
extern "C" __declspec(naked) void tramp_glNormal3iv() { __asm { jmp dword ptr [p_glNormal3iv] } }
extern "C" __declspec(naked) void tramp_glNormal3s() { __asm { jmp dword ptr [p_glNormal3s] } }
extern "C" __declspec(naked) void tramp_glNormal3sv() { __asm { jmp dword ptr [p_glNormal3sv] } }
extern "C" __declspec(naked) void tramp_glNormalPointer() { __asm { jmp dword ptr [p_glNormalPointer] } }
extern "C" __declspec(naked) void tramp_glPassThrough() { __asm { jmp dword ptr [p_glPassThrough] } }
extern "C" __declspec(naked) void tramp_glPixelMapfv() { __asm { jmp dword ptr [p_glPixelMapfv] } }
extern "C" __declspec(naked) void tramp_glPixelMapuiv() { __asm { jmp dword ptr [p_glPixelMapuiv] } }
extern "C" __declspec(naked) void tramp_glPixelMapusv() { __asm { jmp dword ptr [p_glPixelMapusv] } }
extern "C" __declspec(naked) void tramp_glPixelStoref() { __asm { jmp dword ptr [p_glPixelStoref] } }
extern "C" __declspec(naked) void tramp_glPixelStorei() { __asm { jmp dword ptr [p_glPixelStorei] } }
extern "C" __declspec(naked) void tramp_glPixelTransferf() { __asm { jmp dword ptr [p_glPixelTransferf] } }
extern "C" __declspec(naked) void tramp_glPixelTransferi() { __asm { jmp dword ptr [p_glPixelTransferi] } }
extern "C" __declspec(naked) void tramp_glPixelZoom() { __asm { jmp dword ptr [p_glPixelZoom] } }
extern "C" __declspec(naked) void tramp_glPointSize() { __asm { jmp dword ptr [p_glPointSize] } }
extern "C" __declspec(naked) void tramp_glPolygonMode() { __asm { jmp dword ptr [p_glPolygonMode] } }
extern "C" __declspec(naked) void tramp_glPolygonOffset() { __asm { jmp dword ptr [p_glPolygonOffset] } }
extern "C" __declspec(naked) void tramp_glPolygonStipple() { __asm { jmp dword ptr [p_glPolygonStipple] } }
extern "C" __declspec(naked) void tramp_glPopAttrib() { __asm { jmp dword ptr [p_glPopAttrib] } }
extern "C" __declspec(naked) void tramp_glPopClientAttrib() { __asm { jmp dword ptr [p_glPopClientAttrib] } }
extern "C" __declspec(naked) void tramp_glPopMatrix() { __asm { jmp dword ptr [p_glPopMatrix] } }
extern "C" __declspec(naked) void tramp_glPopName() { __asm { jmp dword ptr [p_glPopName] } }
extern "C" __declspec(naked) void tramp_glPrioritizeTextures() { __asm { jmp dword ptr [p_glPrioritizeTextures] } }
extern "C" __declspec(naked) void tramp_glPushAttrib() { __asm { jmp dword ptr [p_glPushAttrib] } }
extern "C" __declspec(naked) void tramp_glPushClientAttrib() { __asm { jmp dword ptr [p_glPushClientAttrib] } }
extern "C" __declspec(naked) void tramp_glPushMatrix() { __asm { jmp dword ptr [p_glPushMatrix] } }
extern "C" __declspec(naked) void tramp_glPushName() { __asm { jmp dword ptr [p_glPushName] } }
extern "C" __declspec(naked) void tramp_glRasterPos2d() { __asm { jmp dword ptr [p_glRasterPos2d] } }
extern "C" __declspec(naked) void tramp_glRasterPos2dv() { __asm { jmp dword ptr [p_glRasterPos2dv] } }
extern "C" __declspec(naked) void tramp_glRasterPos2f() { __asm { jmp dword ptr [p_glRasterPos2f] } }
extern "C" __declspec(naked) void tramp_glRasterPos2fv() { __asm { jmp dword ptr [p_glRasterPos2fv] } }
extern "C" __declspec(naked) void tramp_glRasterPos2i() { __asm { jmp dword ptr [p_glRasterPos2i] } }
extern "C" __declspec(naked) void tramp_glRasterPos2iv() { __asm { jmp dword ptr [p_glRasterPos2iv] } }
extern "C" __declspec(naked) void tramp_glRasterPos2s() { __asm { jmp dword ptr [p_glRasterPos2s] } }
extern "C" __declspec(naked) void tramp_glRasterPos2sv() { __asm { jmp dword ptr [p_glRasterPos2sv] } }
extern "C" __declspec(naked) void tramp_glRasterPos3d() { __asm { jmp dword ptr [p_glRasterPos3d] } }
extern "C" __declspec(naked) void tramp_glRasterPos3dv() { __asm { jmp dword ptr [p_glRasterPos3dv] } }
extern "C" __declspec(naked) void tramp_glRasterPos3f() { __asm { jmp dword ptr [p_glRasterPos3f] } }
extern "C" __declspec(naked) void tramp_glRasterPos3fv() { __asm { jmp dword ptr [p_glRasterPos3fv] } }
extern "C" __declspec(naked) void tramp_glRasterPos3i() { __asm { jmp dword ptr [p_glRasterPos3i] } }
extern "C" __declspec(naked) void tramp_glRasterPos3iv() { __asm { jmp dword ptr [p_glRasterPos3iv] } }
extern "C" __declspec(naked) void tramp_glRasterPos3s() { __asm { jmp dword ptr [p_glRasterPos3s] } }
extern "C" __declspec(naked) void tramp_glRasterPos3sv() { __asm { jmp dword ptr [p_glRasterPos3sv] } }
extern "C" __declspec(naked) void tramp_glRasterPos4d() { __asm { jmp dword ptr [p_glRasterPos4d] } }
extern "C" __declspec(naked) void tramp_glRasterPos4dv() { __asm { jmp dword ptr [p_glRasterPos4dv] } }
extern "C" __declspec(naked) void tramp_glRasterPos4f() { __asm { jmp dword ptr [p_glRasterPos4f] } }
extern "C" __declspec(naked) void tramp_glRasterPos4fv() { __asm { jmp dword ptr [p_glRasterPos4fv] } }
extern "C" __declspec(naked) void tramp_glRasterPos4i() { __asm { jmp dword ptr [p_glRasterPos4i] } }
extern "C" __declspec(naked) void tramp_glRasterPos4iv() { __asm { jmp dword ptr [p_glRasterPos4iv] } }
extern "C" __declspec(naked) void tramp_glRasterPos4s() { __asm { jmp dword ptr [p_glRasterPos4s] } }
extern "C" __declspec(naked) void tramp_glRasterPos4sv() { __asm { jmp dword ptr [p_glRasterPos4sv] } }
extern "C" __declspec(naked) void tramp_glReadBuffer() { __asm { jmp dword ptr [p_glReadBuffer] } }
extern "C" __declspec(naked) void tramp_glReadPixels() { __asm { jmp dword ptr [p_glReadPixels] } }
extern "C" __declspec(naked) void tramp_glRectd() { __asm { jmp dword ptr [p_glRectd] } }
extern "C" __declspec(naked) void tramp_glRectdv() { __asm { jmp dword ptr [p_glRectdv] } }
extern "C" __declspec(naked) void tramp_glRectf() { __asm { jmp dword ptr [p_glRectf] } }
extern "C" __declspec(naked) void tramp_glRectfv() { __asm { jmp dword ptr [p_glRectfv] } }
extern "C" __declspec(naked) void tramp_glRecti() { __asm { jmp dword ptr [p_glRecti] } }
extern "C" __declspec(naked) void tramp_glRectiv() { __asm { jmp dword ptr [p_glRectiv] } }
extern "C" __declspec(naked) void tramp_glRects() { __asm { jmp dword ptr [p_glRects] } }
extern "C" __declspec(naked) void tramp_glRectsv() { __asm { jmp dword ptr [p_glRectsv] } }
extern "C" __declspec(naked) void tramp_glRenderMode() { __asm { jmp dword ptr [p_glRenderMode] } }
extern "C" __declspec(naked) void tramp_glRotated() { __asm { jmp dword ptr [p_glRotated] } }
extern "C" __declspec(naked) void tramp_glRotatef() { __asm { jmp dword ptr [p_glRotatef] } }
extern "C" __declspec(naked) void tramp_glScaled() { __asm { jmp dword ptr [p_glScaled] } }
extern "C" __declspec(naked) void tramp_glScalef() { __asm { jmp dword ptr [p_glScalef] } }
extern "C" __declspec(naked) void tramp_glSelectBuffer() { __asm { jmp dword ptr [p_glSelectBuffer] } }
extern "C" __declspec(naked) void tramp_glShadeModel() { __asm { jmp dword ptr [p_glShadeModel] } }
extern "C" __declspec(naked) void tramp_glStencilFunc() { __asm { jmp dword ptr [p_glStencilFunc] } }
extern "C" __declspec(naked) void tramp_glStencilMask() { __asm { jmp dword ptr [p_glStencilMask] } }
extern "C" __declspec(naked) void tramp_glStencilOp() { __asm { jmp dword ptr [p_glStencilOp] } }
extern "C" __declspec(naked) void tramp_glTexCoord1d() { __asm { jmp dword ptr [p_glTexCoord1d] } }
extern "C" __declspec(naked) void tramp_glTexCoord1dv() { __asm { jmp dword ptr [p_glTexCoord1dv] } }
extern "C" __declspec(naked) void tramp_glTexCoord1f() { __asm { jmp dword ptr [p_glTexCoord1f] } }
extern "C" __declspec(naked) void tramp_glTexCoord1fv() { __asm { jmp dword ptr [p_glTexCoord1fv] } }
extern "C" __declspec(naked) void tramp_glTexCoord1i() { __asm { jmp dword ptr [p_glTexCoord1i] } }
extern "C" __declspec(naked) void tramp_glTexCoord1iv() { __asm { jmp dword ptr [p_glTexCoord1iv] } }
extern "C" __declspec(naked) void tramp_glTexCoord1s() { __asm { jmp dword ptr [p_glTexCoord1s] } }
extern "C" __declspec(naked) void tramp_glTexCoord1sv() { __asm { jmp dword ptr [p_glTexCoord1sv] } }
extern "C" __declspec(naked) void tramp_glTexCoord2d() { __asm { jmp dword ptr [p_glTexCoord2d] } }
extern "C" __declspec(naked) void tramp_glTexCoord2dv() { __asm { jmp dword ptr [p_glTexCoord2dv] } }
extern "C" __declspec(naked) void tramp_glTexCoord2f() { __asm { jmp dword ptr [p_glTexCoord2f] } }
extern "C" __declspec(naked) void tramp_glTexCoord2fv() { __asm { jmp dword ptr [p_glTexCoord2fv] } }
extern "C" __declspec(naked) void tramp_glTexCoord2i() { __asm { jmp dword ptr [p_glTexCoord2i] } }
extern "C" __declspec(naked) void tramp_glTexCoord2iv() { __asm { jmp dword ptr [p_glTexCoord2iv] } }
extern "C" __declspec(naked) void tramp_glTexCoord2s() { __asm { jmp dword ptr [p_glTexCoord2s] } }
extern "C" __declspec(naked) void tramp_glTexCoord2sv() { __asm { jmp dword ptr [p_glTexCoord2sv] } }
extern "C" __declspec(naked) void tramp_glTexCoord3d() { __asm { jmp dword ptr [p_glTexCoord3d] } }
extern "C" __declspec(naked) void tramp_glTexCoord3dv() { __asm { jmp dword ptr [p_glTexCoord3dv] } }
extern "C" __declspec(naked) void tramp_glTexCoord3f() { __asm { jmp dword ptr [p_glTexCoord3f] } }
extern "C" __declspec(naked) void tramp_glTexCoord3fv() { __asm { jmp dword ptr [p_glTexCoord3fv] } }
extern "C" __declspec(naked) void tramp_glTexCoord3i() { __asm { jmp dword ptr [p_glTexCoord3i] } }
extern "C" __declspec(naked) void tramp_glTexCoord3iv() { __asm { jmp dword ptr [p_glTexCoord3iv] } }
extern "C" __declspec(naked) void tramp_glTexCoord3s() { __asm { jmp dword ptr [p_glTexCoord3s] } }
extern "C" __declspec(naked) void tramp_glTexCoord3sv() { __asm { jmp dword ptr [p_glTexCoord3sv] } }
extern "C" __declspec(naked) void tramp_glTexCoord4d() { __asm { jmp dword ptr [p_glTexCoord4d] } }
extern "C" __declspec(naked) void tramp_glTexCoord4dv() { __asm { jmp dword ptr [p_glTexCoord4dv] } }
extern "C" __declspec(naked) void tramp_glTexCoord4f() { __asm { jmp dword ptr [p_glTexCoord4f] } }
extern "C" __declspec(naked) void tramp_glTexCoord4fv() { __asm { jmp dword ptr [p_glTexCoord4fv] } }
extern "C" __declspec(naked) void tramp_glTexCoord4i() { __asm { jmp dword ptr [p_glTexCoord4i] } }
extern "C" __declspec(naked) void tramp_glTexCoord4iv() { __asm { jmp dword ptr [p_glTexCoord4iv] } }
extern "C" __declspec(naked) void tramp_glTexCoord4s() { __asm { jmp dword ptr [p_glTexCoord4s] } }
extern "C" __declspec(naked) void tramp_glTexCoord4sv() { __asm { jmp dword ptr [p_glTexCoord4sv] } }
extern "C" __declspec(naked) void tramp_glTexCoordPointer() { __asm { jmp dword ptr [p_glTexCoordPointer] } }
extern "C" __declspec(naked) void tramp_glTexEnvf() { __asm { jmp dword ptr [p_glTexEnvf] } }
extern "C" __declspec(naked) void tramp_glTexEnvfv() { __asm { jmp dword ptr [p_glTexEnvfv] } }
extern "C" __declspec(naked) void tramp_glTexEnvi() { __asm { jmp dword ptr [p_glTexEnvi] } }
extern "C" __declspec(naked) void tramp_glTexEnviv() { __asm { jmp dword ptr [p_glTexEnviv] } }
extern "C" __declspec(naked) void tramp_glTexGend() { __asm { jmp dword ptr [p_glTexGend] } }
extern "C" __declspec(naked) void tramp_glTexGendv() { __asm { jmp dword ptr [p_glTexGendv] } }
extern "C" __declspec(naked) void tramp_glTexGenf() { __asm { jmp dword ptr [p_glTexGenf] } }
extern "C" __declspec(naked) void tramp_glTexGenfv() { __asm { jmp dword ptr [p_glTexGenfv] } }
extern "C" __declspec(naked) void tramp_glTexGeni() { __asm { jmp dword ptr [p_glTexGeni] } }
extern "C" __declspec(naked) void tramp_glTexGeniv() { __asm { jmp dword ptr [p_glTexGeniv] } }
extern "C" __declspec(naked) void tramp_glTexImage1D() { __asm { jmp dword ptr [p_glTexImage1D] } }
extern "C" __declspec(naked) void tramp_glTexImage2D() { __asm { jmp dword ptr [p_glTexImage2D] } }
extern "C" __declspec(naked) void tramp_glTexParameterf() { __asm { jmp dword ptr [p_glTexParameterf] } }
extern "C" __declspec(naked) void tramp_glTexParameterfv() { __asm { jmp dword ptr [p_glTexParameterfv] } }
extern "C" __declspec(naked) void tramp_glTexParameteri() { __asm { jmp dword ptr [p_glTexParameteri] } }
extern "C" __declspec(naked) void tramp_glTexParameteriv() { __asm { jmp dword ptr [p_glTexParameteriv] } }
extern "C" __declspec(naked) void tramp_glTexSubImage1D() { __asm { jmp dword ptr [p_glTexSubImage1D] } }
extern "C" __declspec(naked) void tramp_glTexSubImage2D() { __asm { jmp dword ptr [p_glTexSubImage2D] } }
extern "C" __declspec(naked) void tramp_glTranslated() { __asm { jmp dword ptr [p_glTranslated] } }
extern "C" __declspec(naked) void tramp_glTranslatef() { __asm { jmp dword ptr [p_glTranslatef] } }
extern "C" __declspec(naked) void tramp_glVertex2d() { __asm { jmp dword ptr [p_glVertex2d] } }
extern "C" __declspec(naked) void tramp_glVertex2dv() { __asm { jmp dword ptr [p_glVertex2dv] } }
extern "C" __declspec(naked) void tramp_glVertex2f() { __asm { jmp dword ptr [p_glVertex2f] } }
extern "C" __declspec(naked) void tramp_glVertex2fv() { __asm { jmp dword ptr [p_glVertex2fv] } }
extern "C" __declspec(naked) void tramp_glVertex2i() { __asm { jmp dword ptr [p_glVertex2i] } }
extern "C" __declspec(naked) void tramp_glVertex2iv() { __asm { jmp dword ptr [p_glVertex2iv] } }
extern "C" __declspec(naked) void tramp_glVertex2s() { __asm { jmp dword ptr [p_glVertex2s] } }
extern "C" __declspec(naked) void tramp_glVertex2sv() { __asm { jmp dword ptr [p_glVertex2sv] } }
extern "C" __declspec(naked) void tramp_glVertex3d() { __asm { jmp dword ptr [p_glVertex3d] } }
extern "C" __declspec(naked) void tramp_glVertex3dv() { __asm { jmp dword ptr [p_glVertex3dv] } }
extern "C" __declspec(naked) void tramp_glVertex3f() { __asm { jmp dword ptr [p_glVertex3f] } }
extern "C" __declspec(naked) void tramp_glVertex3fv() { __asm { jmp dword ptr [p_glVertex3fv] } }
extern "C" __declspec(naked) void tramp_glVertex3i() { __asm { jmp dword ptr [p_glVertex3i] } }
extern "C" __declspec(naked) void tramp_glVertex3iv() { __asm { jmp dword ptr [p_glVertex3iv] } }
extern "C" __declspec(naked) void tramp_glVertex3s() { __asm { jmp dword ptr [p_glVertex3s] } }
extern "C" __declspec(naked) void tramp_glVertex3sv() { __asm { jmp dword ptr [p_glVertex3sv] } }
extern "C" __declspec(naked) void tramp_glVertex4d() { __asm { jmp dword ptr [p_glVertex4d] } }
extern "C" __declspec(naked) void tramp_glVertex4dv() { __asm { jmp dword ptr [p_glVertex4dv] } }
extern "C" __declspec(naked) void tramp_glVertex4f() { __asm { jmp dword ptr [p_glVertex4f] } }
extern "C" __declspec(naked) void tramp_glVertex4fv() { __asm { jmp dword ptr [p_glVertex4fv] } }
extern "C" __declspec(naked) void tramp_glVertex4i() { __asm { jmp dword ptr [p_glVertex4i] } }
extern "C" __declspec(naked) void tramp_glVertex4iv() { __asm { jmp dword ptr [p_glVertex4iv] } }
extern "C" __declspec(naked) void tramp_glVertex4s() { __asm { jmp dword ptr [p_glVertex4s] } }
extern "C" __declspec(naked) void tramp_glVertex4sv() { __asm { jmp dword ptr [p_glVertex4sv] } }
extern "C" __declspec(naked) void tramp_glVertexPointer() { __asm { jmp dword ptr [p_glVertexPointer] } }
extern "C" __declspec(naked) void tramp_wglChoosePixelFormat() { __asm { jmp dword ptr [p_wglChoosePixelFormat] } }
extern "C" __declspec(naked) void tramp_wglCopyContext() { __asm { jmp dword ptr [p_wglCopyContext] } }
extern "C" __declspec(naked) void tramp_wglCreateContext() { __asm { jmp dword ptr [p_wglCreateContext] } }
extern "C" __declspec(naked) void tramp_wglCreateLayerContext() { __asm { jmp dword ptr [p_wglCreateLayerContext] } }
extern "C" __declspec(naked) void tramp_wglDeleteContext() { __asm { jmp dword ptr [p_wglDeleteContext] } }
extern "C" __declspec(naked) void tramp_wglDescribeLayerPlane() { __asm { jmp dword ptr [p_wglDescribeLayerPlane] } }
extern "C" __declspec(naked) void tramp_wglDescribePixelFormat() { __asm { jmp dword ptr [p_wglDescribePixelFormat] } }
extern "C" __declspec(naked) void tramp_wglGetCurrentContext() { __asm { jmp dword ptr [p_wglGetCurrentContext] } }
extern "C" __declspec(naked) void tramp_wglGetCurrentDC() { __asm { jmp dword ptr [p_wglGetCurrentDC] } }
extern "C" __declspec(naked) void tramp_wglGetDefaultProcAddress() { __asm { jmp dword ptr [p_wglGetDefaultProcAddress] } }
extern "C" __declspec(naked) void tramp_wglGetLayerPaletteEntries() { __asm { jmp dword ptr [p_wglGetLayerPaletteEntries] } }
extern "C" __declspec(naked) void tramp_wglGetPixelFormat() { __asm { jmp dword ptr [p_wglGetPixelFormat] } }
extern "C" __declspec(naked) void tramp_wglRealizeLayerPalette() { __asm { jmp dword ptr [p_wglRealizeLayerPalette] } }
extern "C" __declspec(naked) void tramp_wglSetLayerPaletteEntries() { __asm { jmp dword ptr [p_wglSetLayerPaletteEntries] } }
extern "C" __declspec(naked) void tramp_wglSetPixelFormat() { __asm { jmp dword ptr [p_wglSetPixelFormat] } }
extern "C" __declspec(naked) void tramp_wglShareLists() { __asm { jmp dword ptr [p_wglShareLists] } }
extern "C" __declspec(naked) void tramp_wglSwapBuffers() { __asm { jmp dword ptr [p_wglSwapBuffers] } }
extern "C" __declspec(naked) void tramp_wglSwapLayerBuffers() { __asm { jmp dword ptr [p_wglSwapLayerBuffers] } }
extern "C" __declspec(naked) void tramp_wglSwapMultipleBuffers() { __asm { jmp dword ptr [p_wglSwapMultipleBuffers] } }
extern "C" __declspec(naked) void tramp_wglUseFontBitmapsA() { __asm { jmp dword ptr [p_wglUseFontBitmapsA] } }
extern "C" __declspec(naked) void tramp_wglUseFontBitmapsW() { __asm { jmp dword ptr [p_wglUseFontBitmapsW] } }
extern "C" __declspec(naked) void tramp_wglUseFontOutlinesA() { __asm { jmp dword ptr [p_wglUseFontOutlinesA] } }
extern "C" __declspec(naked) void tramp_wglUseFontOutlinesW() { __asm { jmp dword ptr [p_wglUseFontOutlinesW] } }

void InitForwards(HMODULE real)
{
    p_GlmfBeginGlsBlock = GetProcAddress(real, "GlmfBeginGlsBlock");
    p_GlmfCloseMetaFile = GetProcAddress(real, "GlmfCloseMetaFile");
    p_GlmfEndGlsBlock = GetProcAddress(real, "GlmfEndGlsBlock");
    p_GlmfEndPlayback = GetProcAddress(real, "GlmfEndPlayback");
    p_GlmfInitPlayback = GetProcAddress(real, "GlmfInitPlayback");
    p_GlmfPlayGlsRecord = GetProcAddress(real, "GlmfPlayGlsRecord");
    p_glAccum = GetProcAddress(real, "glAccum");
    p_glAlphaFunc = GetProcAddress(real, "glAlphaFunc");
    p_glAreTexturesResident = GetProcAddress(real, "glAreTexturesResident");
    p_glArrayElement = GetProcAddress(real, "glArrayElement");
    p_glBegin = GetProcAddress(real, "glBegin");
    p_glBindTexture = GetProcAddress(real, "glBindTexture");
    p_glBitmap = GetProcAddress(real, "glBitmap");
    p_glBlendFunc = GetProcAddress(real, "glBlendFunc");
    p_glCallList = GetProcAddress(real, "glCallList");
    p_glCallLists = GetProcAddress(real, "glCallLists");
    p_glClearAccum = GetProcAddress(real, "glClearAccum");
    p_glClearColor = GetProcAddress(real, "glClearColor");
    p_glClearDepth = GetProcAddress(real, "glClearDepth");
    p_glClearIndex = GetProcAddress(real, "glClearIndex");
    p_glClearStencil = GetProcAddress(real, "glClearStencil");
    p_glClipPlane = GetProcAddress(real, "glClipPlane");
    p_glColor3b = GetProcAddress(real, "glColor3b");
    p_glColor3bv = GetProcAddress(real, "glColor3bv");
    p_glColor3d = GetProcAddress(real, "glColor3d");
    p_glColor3dv = GetProcAddress(real, "glColor3dv");
    p_glColor3f = GetProcAddress(real, "glColor3f");
    p_glColor3fv = GetProcAddress(real, "glColor3fv");
    p_glColor3i = GetProcAddress(real, "glColor3i");
    p_glColor3iv = GetProcAddress(real, "glColor3iv");
    p_glColor3s = GetProcAddress(real, "glColor3s");
    p_glColor3sv = GetProcAddress(real, "glColor3sv");
    p_glColor3ub = GetProcAddress(real, "glColor3ub");
    p_glColor3ubv = GetProcAddress(real, "glColor3ubv");
    p_glColor3ui = GetProcAddress(real, "glColor3ui");
    p_glColor3uiv = GetProcAddress(real, "glColor3uiv");
    p_glColor3us = GetProcAddress(real, "glColor3us");
    p_glColor3usv = GetProcAddress(real, "glColor3usv");
    p_glColor4b = GetProcAddress(real, "glColor4b");
    p_glColor4bv = GetProcAddress(real, "glColor4bv");
    p_glColor4d = GetProcAddress(real, "glColor4d");
    p_glColor4dv = GetProcAddress(real, "glColor4dv");
    p_glColor4f = GetProcAddress(real, "glColor4f");
    p_glColor4fv = GetProcAddress(real, "glColor4fv");
    p_glColor4i = GetProcAddress(real, "glColor4i");
    p_glColor4iv = GetProcAddress(real, "glColor4iv");
    p_glColor4s = GetProcAddress(real, "glColor4s");
    p_glColor4sv = GetProcAddress(real, "glColor4sv");
    p_glColor4ub = GetProcAddress(real, "glColor4ub");
    p_glColor4ubv = GetProcAddress(real, "glColor4ubv");
    p_glColor4ui = GetProcAddress(real, "glColor4ui");
    p_glColor4uiv = GetProcAddress(real, "glColor4uiv");
    p_glColor4us = GetProcAddress(real, "glColor4us");
    p_glColor4usv = GetProcAddress(real, "glColor4usv");
    p_glColorMask = GetProcAddress(real, "glColorMask");
    p_glColorMaterial = GetProcAddress(real, "glColorMaterial");
    p_glColorPointer = GetProcAddress(real, "glColorPointer");
    p_glCopyPixels = GetProcAddress(real, "glCopyPixels");
    p_glCopyTexImage1D = GetProcAddress(real, "glCopyTexImage1D");
    p_glCopyTexImage2D = GetProcAddress(real, "glCopyTexImage2D");
    p_glCopyTexSubImage1D = GetProcAddress(real, "glCopyTexSubImage1D");
    p_glCopyTexSubImage2D = GetProcAddress(real, "glCopyTexSubImage2D");
    p_glCullFace = GetProcAddress(real, "glCullFace");
    p_glDebugEntry = GetProcAddress(real, "glDebugEntry");
    p_glDeleteLists = GetProcAddress(real, "glDeleteLists");
    p_glDeleteTextures = GetProcAddress(real, "glDeleteTextures");
    p_glDepthFunc = GetProcAddress(real, "glDepthFunc");
    p_glDepthMask = GetProcAddress(real, "glDepthMask");
    p_glDepthRange = GetProcAddress(real, "glDepthRange");
    p_glDisable = GetProcAddress(real, "glDisable");
    p_glDisableClientState = GetProcAddress(real, "glDisableClientState");
    p_glDrawArrays = GetProcAddress(real, "glDrawArrays");
    p_glDrawBuffer = GetProcAddress(real, "glDrawBuffer");
    p_glDrawElements = GetProcAddress(real, "glDrawElements");
    p_glDrawPixels = GetProcAddress(real, "glDrawPixels");
    p_glEdgeFlag = GetProcAddress(real, "glEdgeFlag");
    p_glEdgeFlagPointer = GetProcAddress(real, "glEdgeFlagPointer");
    p_glEdgeFlagv = GetProcAddress(real, "glEdgeFlagv");
    p_glEnable = GetProcAddress(real, "glEnable");
    p_glEnableClientState = GetProcAddress(real, "glEnableClientState");
    p_glEnd = GetProcAddress(real, "glEnd");
    p_glEndList = GetProcAddress(real, "glEndList");
    p_glEvalCoord1d = GetProcAddress(real, "glEvalCoord1d");
    p_glEvalCoord1dv = GetProcAddress(real, "glEvalCoord1dv");
    p_glEvalCoord1f = GetProcAddress(real, "glEvalCoord1f");
    p_glEvalCoord1fv = GetProcAddress(real, "glEvalCoord1fv");
    p_glEvalCoord2d = GetProcAddress(real, "glEvalCoord2d");
    p_glEvalCoord2dv = GetProcAddress(real, "glEvalCoord2dv");
    p_glEvalCoord2f = GetProcAddress(real, "glEvalCoord2f");
    p_glEvalCoord2fv = GetProcAddress(real, "glEvalCoord2fv");
    p_glEvalMesh1 = GetProcAddress(real, "glEvalMesh1");
    p_glEvalMesh2 = GetProcAddress(real, "glEvalMesh2");
    p_glEvalPoint1 = GetProcAddress(real, "glEvalPoint1");
    p_glEvalPoint2 = GetProcAddress(real, "glEvalPoint2");
    p_glFeedbackBuffer = GetProcAddress(real, "glFeedbackBuffer");
    p_glFinish = GetProcAddress(real, "glFinish");
    p_glFlush = GetProcAddress(real, "glFlush");
    p_glFogf = GetProcAddress(real, "glFogf");
    p_glFogfv = GetProcAddress(real, "glFogfv");
    p_glFogi = GetProcAddress(real, "glFogi");
    p_glFogiv = GetProcAddress(real, "glFogiv");
    p_glFrontFace = GetProcAddress(real, "glFrontFace");
    p_glFrustum = GetProcAddress(real, "glFrustum");
    p_glGenLists = GetProcAddress(real, "glGenLists");
    p_glGenTextures = GetProcAddress(real, "glGenTextures");
    p_glGetBooleanv = GetProcAddress(real, "glGetBooleanv");
    p_glGetClipPlane = GetProcAddress(real, "glGetClipPlane");
    p_glGetDoublev = GetProcAddress(real, "glGetDoublev");
    p_glGetError = GetProcAddress(real, "glGetError");
    p_glGetFloatv = GetProcAddress(real, "glGetFloatv");
    p_glGetLightfv = GetProcAddress(real, "glGetLightfv");
    p_glGetLightiv = GetProcAddress(real, "glGetLightiv");
    p_glGetMapdv = GetProcAddress(real, "glGetMapdv");
    p_glGetMapfv = GetProcAddress(real, "glGetMapfv");
    p_glGetMapiv = GetProcAddress(real, "glGetMapiv");
    p_glGetMaterialfv = GetProcAddress(real, "glGetMaterialfv");
    p_glGetMaterialiv = GetProcAddress(real, "glGetMaterialiv");
    p_glGetPixelMapfv = GetProcAddress(real, "glGetPixelMapfv");
    p_glGetPixelMapuiv = GetProcAddress(real, "glGetPixelMapuiv");
    p_glGetPixelMapusv = GetProcAddress(real, "glGetPixelMapusv");
    p_glGetPointerv = GetProcAddress(real, "glGetPointerv");
    p_glGetPolygonStipple = GetProcAddress(real, "glGetPolygonStipple");
    p_glGetString = GetProcAddress(real, "glGetString");
    p_glGetTexEnvfv = GetProcAddress(real, "glGetTexEnvfv");
    p_glGetTexEnviv = GetProcAddress(real, "glGetTexEnviv");
    p_glGetTexGendv = GetProcAddress(real, "glGetTexGendv");
    p_glGetTexGenfv = GetProcAddress(real, "glGetTexGenfv");
    p_glGetTexGeniv = GetProcAddress(real, "glGetTexGeniv");
    p_glGetTexImage = GetProcAddress(real, "glGetTexImage");
    p_glGetTexLevelParameterfv = GetProcAddress(real, "glGetTexLevelParameterfv");
    p_glGetTexLevelParameteriv = GetProcAddress(real, "glGetTexLevelParameteriv");
    p_glGetTexParameterfv = GetProcAddress(real, "glGetTexParameterfv");
    p_glGetTexParameteriv = GetProcAddress(real, "glGetTexParameteriv");
    p_glHint = GetProcAddress(real, "glHint");
    p_glIndexMask = GetProcAddress(real, "glIndexMask");
    p_glIndexPointer = GetProcAddress(real, "glIndexPointer");
    p_glIndexd = GetProcAddress(real, "glIndexd");
    p_glIndexdv = GetProcAddress(real, "glIndexdv");
    p_glIndexf = GetProcAddress(real, "glIndexf");
    p_glIndexfv = GetProcAddress(real, "glIndexfv");
    p_glIndexi = GetProcAddress(real, "glIndexi");
    p_glIndexiv = GetProcAddress(real, "glIndexiv");
    p_glIndexs = GetProcAddress(real, "glIndexs");
    p_glIndexsv = GetProcAddress(real, "glIndexsv");
    p_glIndexub = GetProcAddress(real, "glIndexub");
    p_glIndexubv = GetProcAddress(real, "glIndexubv");
    p_glInitNames = GetProcAddress(real, "glInitNames");
    p_glInterleavedArrays = GetProcAddress(real, "glInterleavedArrays");
    p_glIsEnabled = GetProcAddress(real, "glIsEnabled");
    p_glIsList = GetProcAddress(real, "glIsList");
    p_glIsTexture = GetProcAddress(real, "glIsTexture");
    p_glLightModelf = GetProcAddress(real, "glLightModelf");
    p_glLightModelfv = GetProcAddress(real, "glLightModelfv");
    p_glLightModeli = GetProcAddress(real, "glLightModeli");
    p_glLightModeliv = GetProcAddress(real, "glLightModeliv");
    p_glLightf = GetProcAddress(real, "glLightf");
    p_glLightfv = GetProcAddress(real, "glLightfv");
    p_glLighti = GetProcAddress(real, "glLighti");
    p_glLightiv = GetProcAddress(real, "glLightiv");
    p_glLineStipple = GetProcAddress(real, "glLineStipple");
    p_glLineWidth = GetProcAddress(real, "glLineWidth");
    p_glListBase = GetProcAddress(real, "glListBase");
    p_glLoadIdentity = GetProcAddress(real, "glLoadIdentity");
    p_glLoadMatrixd = GetProcAddress(real, "glLoadMatrixd");
    p_glLoadMatrixf = GetProcAddress(real, "glLoadMatrixf");
    p_glLoadName = GetProcAddress(real, "glLoadName");
    p_glLogicOp = GetProcAddress(real, "glLogicOp");
    p_glMap1d = GetProcAddress(real, "glMap1d");
    p_glMap1f = GetProcAddress(real, "glMap1f");
    p_glMap2d = GetProcAddress(real, "glMap2d");
    p_glMap2f = GetProcAddress(real, "glMap2f");
    p_glMapGrid1d = GetProcAddress(real, "glMapGrid1d");
    p_glMapGrid1f = GetProcAddress(real, "glMapGrid1f");
    p_glMapGrid2d = GetProcAddress(real, "glMapGrid2d");
    p_glMapGrid2f = GetProcAddress(real, "glMapGrid2f");
    p_glMaterialf = GetProcAddress(real, "glMaterialf");
    p_glMaterialfv = GetProcAddress(real, "glMaterialfv");
    p_glMateriali = GetProcAddress(real, "glMateriali");
    p_glMaterialiv = GetProcAddress(real, "glMaterialiv");
    p_glMatrixMode = GetProcAddress(real, "glMatrixMode");
    p_glMultMatrixd = GetProcAddress(real, "glMultMatrixd");
    p_glMultMatrixf = GetProcAddress(real, "glMultMatrixf");
    p_glNewList = GetProcAddress(real, "glNewList");
    p_glNormal3b = GetProcAddress(real, "glNormal3b");
    p_glNormal3bv = GetProcAddress(real, "glNormal3bv");
    p_glNormal3d = GetProcAddress(real, "glNormal3d");
    p_glNormal3dv = GetProcAddress(real, "glNormal3dv");
    p_glNormal3f = GetProcAddress(real, "glNormal3f");
    p_glNormal3fv = GetProcAddress(real, "glNormal3fv");
    p_glNormal3i = GetProcAddress(real, "glNormal3i");
    p_glNormal3iv = GetProcAddress(real, "glNormal3iv");
    p_glNormal3s = GetProcAddress(real, "glNormal3s");
    p_glNormal3sv = GetProcAddress(real, "glNormal3sv");
    p_glNormalPointer = GetProcAddress(real, "glNormalPointer");
    p_glPassThrough = GetProcAddress(real, "glPassThrough");
    p_glPixelMapfv = GetProcAddress(real, "glPixelMapfv");
    p_glPixelMapuiv = GetProcAddress(real, "glPixelMapuiv");
    p_glPixelMapusv = GetProcAddress(real, "glPixelMapusv");
    p_glPixelStoref = GetProcAddress(real, "glPixelStoref");
    p_glPixelStorei = GetProcAddress(real, "glPixelStorei");
    p_glPixelTransferf = GetProcAddress(real, "glPixelTransferf");
    p_glPixelTransferi = GetProcAddress(real, "glPixelTransferi");
    p_glPixelZoom = GetProcAddress(real, "glPixelZoom");
    p_glPointSize = GetProcAddress(real, "glPointSize");
    p_glPolygonMode = GetProcAddress(real, "glPolygonMode");
    p_glPolygonOffset = GetProcAddress(real, "glPolygonOffset");
    p_glPolygonStipple = GetProcAddress(real, "glPolygonStipple");
    p_glPopAttrib = GetProcAddress(real, "glPopAttrib");
    p_glPopClientAttrib = GetProcAddress(real, "glPopClientAttrib");
    p_glPopMatrix = GetProcAddress(real, "glPopMatrix");
    p_glPopName = GetProcAddress(real, "glPopName");
    p_glPrioritizeTextures = GetProcAddress(real, "glPrioritizeTextures");
    p_glPushAttrib = GetProcAddress(real, "glPushAttrib");
    p_glPushClientAttrib = GetProcAddress(real, "glPushClientAttrib");
    p_glPushMatrix = GetProcAddress(real, "glPushMatrix");
    p_glPushName = GetProcAddress(real, "glPushName");
    p_glRasterPos2d = GetProcAddress(real, "glRasterPos2d");
    p_glRasterPos2dv = GetProcAddress(real, "glRasterPos2dv");
    p_glRasterPos2f = GetProcAddress(real, "glRasterPos2f");
    p_glRasterPos2fv = GetProcAddress(real, "glRasterPos2fv");
    p_glRasterPos2i = GetProcAddress(real, "glRasterPos2i");
    p_glRasterPos2iv = GetProcAddress(real, "glRasterPos2iv");
    p_glRasterPos2s = GetProcAddress(real, "glRasterPos2s");
    p_glRasterPos2sv = GetProcAddress(real, "glRasterPos2sv");
    p_glRasterPos3d = GetProcAddress(real, "glRasterPos3d");
    p_glRasterPos3dv = GetProcAddress(real, "glRasterPos3dv");
    p_glRasterPos3f = GetProcAddress(real, "glRasterPos3f");
    p_glRasterPos3fv = GetProcAddress(real, "glRasterPos3fv");
    p_glRasterPos3i = GetProcAddress(real, "glRasterPos3i");
    p_glRasterPos3iv = GetProcAddress(real, "glRasterPos3iv");
    p_glRasterPos3s = GetProcAddress(real, "glRasterPos3s");
    p_glRasterPos3sv = GetProcAddress(real, "glRasterPos3sv");
    p_glRasterPos4d = GetProcAddress(real, "glRasterPos4d");
    p_glRasterPos4dv = GetProcAddress(real, "glRasterPos4dv");
    p_glRasterPos4f = GetProcAddress(real, "glRasterPos4f");
    p_glRasterPos4fv = GetProcAddress(real, "glRasterPos4fv");
    p_glRasterPos4i = GetProcAddress(real, "glRasterPos4i");
    p_glRasterPos4iv = GetProcAddress(real, "glRasterPos4iv");
    p_glRasterPos4s = GetProcAddress(real, "glRasterPos4s");
    p_glRasterPos4sv = GetProcAddress(real, "glRasterPos4sv");
    p_glReadBuffer = GetProcAddress(real, "glReadBuffer");
    p_glReadPixels = GetProcAddress(real, "glReadPixels");
    p_glRectd = GetProcAddress(real, "glRectd");
    p_glRectdv = GetProcAddress(real, "glRectdv");
    p_glRectf = GetProcAddress(real, "glRectf");
    p_glRectfv = GetProcAddress(real, "glRectfv");
    p_glRecti = GetProcAddress(real, "glRecti");
    p_glRectiv = GetProcAddress(real, "glRectiv");
    p_glRects = GetProcAddress(real, "glRects");
    p_glRectsv = GetProcAddress(real, "glRectsv");
    p_glRenderMode = GetProcAddress(real, "glRenderMode");
    p_glRotated = GetProcAddress(real, "glRotated");
    p_glRotatef = GetProcAddress(real, "glRotatef");
    p_glScaled = GetProcAddress(real, "glScaled");
    p_glScalef = GetProcAddress(real, "glScalef");
    p_glSelectBuffer = GetProcAddress(real, "glSelectBuffer");
    p_glShadeModel = GetProcAddress(real, "glShadeModel");
    p_glStencilFunc = GetProcAddress(real, "glStencilFunc");
    p_glStencilMask = GetProcAddress(real, "glStencilMask");
    p_glStencilOp = GetProcAddress(real, "glStencilOp");
    p_glTexCoord1d = GetProcAddress(real, "glTexCoord1d");
    p_glTexCoord1dv = GetProcAddress(real, "glTexCoord1dv");
    p_glTexCoord1f = GetProcAddress(real, "glTexCoord1f");
    p_glTexCoord1fv = GetProcAddress(real, "glTexCoord1fv");
    p_glTexCoord1i = GetProcAddress(real, "glTexCoord1i");
    p_glTexCoord1iv = GetProcAddress(real, "glTexCoord1iv");
    p_glTexCoord1s = GetProcAddress(real, "glTexCoord1s");
    p_glTexCoord1sv = GetProcAddress(real, "glTexCoord1sv");
    p_glTexCoord2d = GetProcAddress(real, "glTexCoord2d");
    p_glTexCoord2dv = GetProcAddress(real, "glTexCoord2dv");
    p_glTexCoord2f = GetProcAddress(real, "glTexCoord2f");
    p_glTexCoord2fv = GetProcAddress(real, "glTexCoord2fv");
    p_glTexCoord2i = GetProcAddress(real, "glTexCoord2i");
    p_glTexCoord2iv = GetProcAddress(real, "glTexCoord2iv");
    p_glTexCoord2s = GetProcAddress(real, "glTexCoord2s");
    p_glTexCoord2sv = GetProcAddress(real, "glTexCoord2sv");
    p_glTexCoord3d = GetProcAddress(real, "glTexCoord3d");
    p_glTexCoord3dv = GetProcAddress(real, "glTexCoord3dv");
    p_glTexCoord3f = GetProcAddress(real, "glTexCoord3f");
    p_glTexCoord3fv = GetProcAddress(real, "glTexCoord3fv");
    p_glTexCoord3i = GetProcAddress(real, "glTexCoord3i");
    p_glTexCoord3iv = GetProcAddress(real, "glTexCoord3iv");
    p_glTexCoord3s = GetProcAddress(real, "glTexCoord3s");
    p_glTexCoord3sv = GetProcAddress(real, "glTexCoord3sv");
    p_glTexCoord4d = GetProcAddress(real, "glTexCoord4d");
    p_glTexCoord4dv = GetProcAddress(real, "glTexCoord4dv");
    p_glTexCoord4f = GetProcAddress(real, "glTexCoord4f");
    p_glTexCoord4fv = GetProcAddress(real, "glTexCoord4fv");
    p_glTexCoord4i = GetProcAddress(real, "glTexCoord4i");
    p_glTexCoord4iv = GetProcAddress(real, "glTexCoord4iv");
    p_glTexCoord4s = GetProcAddress(real, "glTexCoord4s");
    p_glTexCoord4sv = GetProcAddress(real, "glTexCoord4sv");
    p_glTexCoordPointer = GetProcAddress(real, "glTexCoordPointer");
    p_glTexEnvf = GetProcAddress(real, "glTexEnvf");
    p_glTexEnvfv = GetProcAddress(real, "glTexEnvfv");
    p_glTexEnvi = GetProcAddress(real, "glTexEnvi");
    p_glTexEnviv = GetProcAddress(real, "glTexEnviv");
    p_glTexGend = GetProcAddress(real, "glTexGend");
    p_glTexGendv = GetProcAddress(real, "glTexGendv");
    p_glTexGenf = GetProcAddress(real, "glTexGenf");
    p_glTexGenfv = GetProcAddress(real, "glTexGenfv");
    p_glTexGeni = GetProcAddress(real, "glTexGeni");
    p_glTexGeniv = GetProcAddress(real, "glTexGeniv");
    p_glTexImage1D = GetProcAddress(real, "glTexImage1D");
    p_glTexImage2D = GetProcAddress(real, "glTexImage2D");
    p_glTexParameterf = GetProcAddress(real, "glTexParameterf");
    p_glTexParameterfv = GetProcAddress(real, "glTexParameterfv");
    p_glTexParameteri = GetProcAddress(real, "glTexParameteri");
    p_glTexParameteriv = GetProcAddress(real, "glTexParameteriv");
    p_glTexSubImage1D = GetProcAddress(real, "glTexSubImage1D");
    p_glTexSubImage2D = GetProcAddress(real, "glTexSubImage2D");
    p_glTranslated = GetProcAddress(real, "glTranslated");
    p_glTranslatef = GetProcAddress(real, "glTranslatef");
    p_glVertex2d = GetProcAddress(real, "glVertex2d");
    p_glVertex2dv = GetProcAddress(real, "glVertex2dv");
    p_glVertex2f = GetProcAddress(real, "glVertex2f");
    p_glVertex2fv = GetProcAddress(real, "glVertex2fv");
    p_glVertex2i = GetProcAddress(real, "glVertex2i");
    p_glVertex2iv = GetProcAddress(real, "glVertex2iv");
    p_glVertex2s = GetProcAddress(real, "glVertex2s");
    p_glVertex2sv = GetProcAddress(real, "glVertex2sv");
    p_glVertex3d = GetProcAddress(real, "glVertex3d");
    p_glVertex3dv = GetProcAddress(real, "glVertex3dv");
    p_glVertex3f = GetProcAddress(real, "glVertex3f");
    p_glVertex3fv = GetProcAddress(real, "glVertex3fv");
    p_glVertex3i = GetProcAddress(real, "glVertex3i");
    p_glVertex3iv = GetProcAddress(real, "glVertex3iv");
    p_glVertex3s = GetProcAddress(real, "glVertex3s");
    p_glVertex3sv = GetProcAddress(real, "glVertex3sv");
    p_glVertex4d = GetProcAddress(real, "glVertex4d");
    p_glVertex4dv = GetProcAddress(real, "glVertex4dv");
    p_glVertex4f = GetProcAddress(real, "glVertex4f");
    p_glVertex4fv = GetProcAddress(real, "glVertex4fv");
    p_glVertex4i = GetProcAddress(real, "glVertex4i");
    p_glVertex4iv = GetProcAddress(real, "glVertex4iv");
    p_glVertex4s = GetProcAddress(real, "glVertex4s");
    p_glVertex4sv = GetProcAddress(real, "glVertex4sv");
    p_glVertexPointer = GetProcAddress(real, "glVertexPointer");
    p_wglChoosePixelFormat = GetProcAddress(real, "wglChoosePixelFormat");
    p_wglCopyContext = GetProcAddress(real, "wglCopyContext");
    p_wglCreateContext = GetProcAddress(real, "wglCreateContext");
    p_wglCreateLayerContext = GetProcAddress(real, "wglCreateLayerContext");
    p_wglDeleteContext = GetProcAddress(real, "wglDeleteContext");
    p_wglDescribeLayerPlane = GetProcAddress(real, "wglDescribeLayerPlane");
    p_wglDescribePixelFormat = GetProcAddress(real, "wglDescribePixelFormat");
    p_wglGetCurrentContext = GetProcAddress(real, "wglGetCurrentContext");
    p_wglGetCurrentDC = GetProcAddress(real, "wglGetCurrentDC");
    p_wglGetDefaultProcAddress = GetProcAddress(real, "wglGetDefaultProcAddress");
    p_wglGetLayerPaletteEntries = GetProcAddress(real, "wglGetLayerPaletteEntries");
    p_wglGetPixelFormat = GetProcAddress(real, "wglGetPixelFormat");
    p_wglRealizeLayerPalette = GetProcAddress(real, "wglRealizeLayerPalette");
    p_wglSetLayerPaletteEntries = GetProcAddress(real, "wglSetLayerPaletteEntries");
    p_wglSetPixelFormat = GetProcAddress(real, "wglSetPixelFormat");
    p_wglShareLists = GetProcAddress(real, "wglShareLists");
    p_wglSwapBuffers = GetProcAddress(real, "wglSwapBuffers");
    p_wglSwapLayerBuffers = GetProcAddress(real, "wglSwapLayerBuffers");
    p_wglSwapMultipleBuffers = GetProcAddress(real, "wglSwapMultipleBuffers");
    p_wglUseFontBitmapsA = GetProcAddress(real, "wglUseFontBitmapsA");
    p_wglUseFontBitmapsW = GetProcAddress(real, "wglUseFontBitmapsW");
    p_wglUseFontOutlinesA = GetProcAddress(real, "wglUseFontOutlinesA");
    p_wglUseFontOutlinesW = GetProcAddress(real, "wglUseFontOutlinesW");
}
