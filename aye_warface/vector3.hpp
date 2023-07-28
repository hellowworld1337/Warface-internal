#pragma once
#include <d3d11.h>
#define ILINE __forceinline
#define RED         0xFFFF0000
#define _ifont SSystemGlobalEnvironment::Singleton()->GetCryFont()->GetFont(("warface"))
#define DrawDx11 SSystemGlobalEnvironment::Singleton()->GetIGame()->GetIGameFramework()->m_pUIDraw
#define  cry_dark_red D3DCOLOR_XRGB(62,119,234)
#define  cry_dark7 D3DCOLOR_XRGB(105,231,141)
typedef __int64	int16;
typedef __int32 int8;
typedef __int32	int32;
typedef __int16 int64;
typedef const char* cchar;
typedef unsigned __int64	uint64;
typedef unsigned int uint49;
typedef unsigned int EntityId;
typedef float  f32;
const static float size_text = 17.f;
enum ui_scroll_type
{
	ui_materia,
	ui_spawn,
	ui_on_off,
	ui_aim_mode,
	ui_aim_body,
	ui_int_slider,
	ui_char_slider,
	ui_priority,
	ui_languege,
	ui_type_weapon,
	ui_aim,
	ui_damag,
	ui_teleport,
	ui_fov,


};
enum eRenderPrimitiveType
{
	eptTriangleList = 0,
	eptTriangleStrip = 1,
	eptLineList = 2,
	eptLineStrip = 3,
	eptPointList = 4,
	eptNumRenderablePrimitives = 5,
	eptMultiTriangleStrips = 5,
	eptHWSkinGroups = 6,
};
template <class IVMT>
IVMT GetVFunc(PVOID pointer, DWORD64 offset) {
	return (*(IVMT**)pointer)[offset / 8];
}
template <class IVMT>
IVMT GetVFuncI(PVOID pointer, DWORD64 offset) {
	return (*(IVMT**)pointer)[offset];
}
template <class IVMT>
IVMT GetVFunc(uintptr_t globalAddress) {
	return (IVMT)(globalAddress);
}
enum type_zero { ZERO };
enum type_min { VMIN };
enum type_max { VMAX };
enum type_identity { IDENTITY };
template <typename F> struct HUDSilhouettesColor_tpl
{
	F r, g, b, a;
};
template <typename F> struct Vec3_tpl {
	F x, y, z;
	ILINE Vec3_tpl(type_zero) : x(0), y(0), z(0) {}
	ILINE Vec3_tpl(type_min);
	ILINE Vec3_tpl(type_max);
	explicit ILINE Vec3_tpl(F f) : x(f), y(f), z(f) { }
	ILINE Vec3_tpl() {
		Vec3_tpl(0, 0, 0);
	}
	ILINE Vec3_tpl(F xt, F yt, F zt) {
		x = xt;
		y = yt;
		z = zt;
	}
	ILINE F length() const { return (F)_sqrt(x * x + y * y + z * z); }
	template<typename T> ILINE F sum(T a) {
		F s = 0;
		for (int i = 0; i < a.size; ++i) s += a[i];
		return s;
	}
	template<typename T> ILINE F dot(T a, T b) { return sum(a * b); }

	ILINE F  distance(Vec3_tpl VecB) { return (F)_sqrt(((x - VecB.x) * (x - VecB.x)) + ((y - VecB.y) * (y - VecB.y)) + ((z - VecB.z) * (z - VecB.z))); }

	ILINE F& operator[](int index) { return ((F*)this)[index]; }
	ILINE F  operator[](int index) const { return ((F*)this)[index]; }

	void operator += (F f) { x += f; y += f; z += f; }
	void operator -= (F f) { x -= f; y -= f; z -= f; }
	ILINE Vec3_tpl<F> operator*(F k) const {
		const Vec3_tpl<F> v = *this;
		return Vec3_tpl<F>(v.x * k, v.y * k, v.z * k);
	}
	ILINE Vec3_tpl& operator /  (F f) { return Vec3_tpl(x / f, y / f, z / f); }
	ILINE Vec3_tpl& operator = (const Vec3_tpl& source) { x = source.x; y = source.y; z = source.z; return *this; }
	ILINE Vec3_tpl& operator = (const F value) { x = value; y = value; z = value;   return *this; }
	ILINE Vec3_tpl& operator * (F f) { return Vec3_tpl(x * f, y * f, z * f); }
	ILINE Vec3_tpl& operator *= (F f) { x *= f; y *= f; z *= f; return *this; }
	ILINE Vec3_tpl& operator /= (F f) { x /= f; y /= f; z /= f; return *this; }

	ILINE friend Vec3_tpl operator + (const Vec3_tpl& vec1, const Vec3_tpl& vec2) { return Vec3_tpl(vec1.x + vec2.x, vec1.y + vec2.y, vec1.z + vec2.z); }
	ILINE friend Vec3_tpl operator + (const Vec3_tpl& vec1, const F* vec2) { return vec1 + Vec3_tpl(vec2); }
	ILINE friend Vec3_tpl operator + (const F* vec1, const Vec3_tpl& vec2) { return Vec3_tpl(vec1) + vec2; }

	ILINE friend Vec3_tpl operator - (const Vec3_tpl& vec1, const Vec3_tpl& vec2) { return Vec3_tpl(vec1.x - vec2.x, vec1.y - vec2.y, vec1.z - vec2.z); }
	ILINE friend Vec3_tpl operator - (const Vec3_tpl& vec1, const F* vec2) { return vec1 - Vec3_tpl(vec2); }
	ILINE friend Vec3_tpl operator - (const F* vec1, const Vec3_tpl& vec2) { return Vec3_tpl(vec1) - vec2; }

	ILINE Vec3_tpl<F>& Set(const F xval, const F yval, const F zval) {
		x = xval; y = yval; z = zval;
		return *this;
	}

	ILINE Vec3_tpl& normalize() {
		F len2 = x * x + y * y + z * z;
		if (len2 > (F)1e-20f) {
			F rlen = _sqrt(len2);
			x *= rlen;
			y *= rlen;
			z *= rlen;
		}
		else Set(0, 0, 1);

		return *this;
	}
	ILINE Vec3_tpl<F> GetNormalized() const {
		FLOAT fInvLen = (FLOAT)isqrt_safe_tpl(x * x + y * y + z * z);
		Vec3_tpl<F> out = *this;
		out.x *= fInvLen;
		out.y *= fInvLen;
		out.z *= fInvLen;
		return out;
	}
};

template <typename F> struct Matrix33_tpl
{
	F m00, m01, m02;
	F m10, m11, m12;
	F m20, m21, m22;

	Matrix33_tpl<F>(const Vec3_tpl<F>& vx, const Vec3_tpl<F>& vy, const Vec3_tpl<F>& vz)
	{
		m00 = (vx.x);
		m01 = (vy.x);
		m02 = (vz.x);

		m10 = (vx.y);
		m11 = (vy.y);
		m12 = (vz.y);

		m20 = (vx.z);
		m21 = (vy.z);
		m22 = (vz.z);
	}
};

template <typename F> struct Quat_tpl
{
	Vec3_tpl<F> v;
	F w;
	friend float operator | (const Quat_tpl<F>& q, const Quat_tpl<F>& p)
	{
		return (q.v.x * p.v.x + q.v.y * p.v.y + q.v.z * p.v.z + q.w * p.w);
	}

	friend Quat_tpl<F> operator - (const Quat_tpl<F>& q, const Quat_tpl<F>& p)
	{
		Quat_tpl<F> ret;
		ret.w = q.w - p.w;

		ret.v.x = q.v.x - p.v.x;
		ret.v.y = q.v.y - p.v.y;
		ret.v.z = q.v.z - p.v.z;

		return ret;
	}

	Quat_tpl<F>() {}

	Quat_tpl<F>(const Matrix33_tpl<F>& m)
	{
		float s, p, tr = m.m00 + m.m11 + m.m22;
		w = 1, v.x = 0, v.y = 0, v.z = 0;

		if (tr > 0)
			s = _sqrt(tr + 1.0f), p = 0.5f / s, w = s * 0.5f, v.x = (m.m21 - m.m12) * p, v.y = (m.m02 - m.m20) * p, v.z = (m.m10 - m.m01) * p;
		else if ((m.m00 >= m.m11) && (m.m00 >= m.m22))
			s = _sqrt(m.m00 - m.m11 - m.m22 + 1.0f), p = 0.5f / s, w = (m.m21 - m.m12) * p, v.x = s * 0.5f, v.y = (m.m10 + m.m01) * p, v.z = (m.m20 + m.m02) * p;
		else if ((m.m11 >= m.m00) && (m.m11 >= m.m22))
			s = _sqrt(m.m11 - m.m22 - m.m00 + 1.0f), p = 0.5f / s, w = (m.m02 - m.m20) * p, v.x = (m.m01 + m.m10) * p, v.y = s * 0.5f, v.z = (m.m21 + m.m12) * p;
		else if ((m.m22 >= m.m00) && (m.m22 >= m.m11))
			s = _sqrt(m.m22 - m.m00 - m.m11 + 1.0f), p = 0.5f / s, w = (m.m10 - m.m01) * p, v.x = (m.m02 + m.m20) * p, v.y = (m.m12 + m.m21) * p, v.z = s * 0.5f;
	}

};

template <typename F> struct QuatT_tpl
{
	Quat_tpl<F> q;
	Vec3_tpl<F> t;
};

template <typename F> struct Matrix34_tpl
{
	F m00, m01, m02, m03;
	F m10, m11, m12, m13;
	F m20, m21, m22, m23;

	template<class F1> explicit Matrix34_tpl(const QuatT_tpl<F1>& q) {
		Vec3_tpl<F1> v2 = q.q.v + q.q.v;
		F1 xx = 1 - v2.x * q.q.v.x;		F1 yy = v2.y * q.q.v.y;		F1 xw = v2.x * q.q.w;
		F1 xy = v2.y * q.q.v.x;			F1 yz = v2.z * q.q.v.y;		F1 yw = v2.y * q.q.w;
		F1 xz = v2.z * q.q.v.x;			F1 zz = v2.z * q.q.v.z;		F1 zw = v2.z * q.q.w;
		m00 = F(1 - yy - zz);			m01 = F(xy - zw);			m02 = F(xz + yw);			m03 = F(q.t.x);
		m10 = F(xy + zw);				m11 = F(xx - zz);			m12 = F(yz - xw);			m13 = F(q.t.y);
		m20 = F(xz - yw);				m21 = F(yz + xw);			m22 = F(xx - yy);			m23 = F(q.t.z);
	}

	Vec3_tpl <F>GetTranslation() const { return Vec3_tpl <F>(m03, m13, m23); }
};
template <typename F> struct Vec2_tpl {
	F x, y;
	Vec2_tpl(type_zero) : x(0), y(0) {}
	explicit Vec2_tpl(F f) : x(f), y(f) {}

	Vec2_tpl() { Vec2_tpl<F>(0, 0); }
	Vec2_tpl(F xt, F yt) { x = xt; y = yt; }

	void Set(F xt, F yt) { x = xt; y = yt; }
};
template <typename F> struct Color_tpl {
public:
	F a, r, g, b;

	Color_tpl() { Color_tpl(0, 0, 0, 0); }
	Color_tpl(int a, int r, int g, int b) { this->a = a; this->r = r; this->g = g; this->b = b; }
	Color_tpl(int r, int g, int b) { this->a = 255; this->r = r; this->g = g; this->b = b; }
	Color_tpl(unsigned long color) { this->b = (color & 0xff); this->g = ((color >> 8) & 0xff); this->r = ((color >> 16) & 0xff); this->a = ((color >> 24) & 0xff); }
	inline float* Base() {
		float fColor[3];
		fColor[0] = this->r / 255.0f;
		fColor[1] = this->g / 255.0f;
		fColor[2] = this->b / 255.0f;
		return &fColor[0];
	}

	inline float rBase() const { return this->r / 255.0f; }
	inline float gBase() const { return this->g / 255.0f; }
	inline float bBase() const { return this->b / 255.0f; }
	inline float aBase() const { return this->a / 255.0f; }

	inline operator unsigned long() const { return (a << 24) | (r << 16) | (g << 8) | b; }
};


typedef Vec3_tpl<float>      Vec3;
typedef Quat_tpl<float>		 Quat;
typedef QuatT_tpl<float>     QuatT;
typedef Matrix34_tpl<float>	 Matrix34;
typedef Vec2_tpl<float>								Vec2;
typedef Color_tpl<unsigned char> ColorB;
typedef Color_tpl<float> ColorF;

struct TKeyName
{
	const char* key;
	TKeyName() { key = ""; }
	TKeyName(const char* _key) { key = _key; };
	operator const char* () const { return key; };
	const char* c_str() const { return key; }
};
struct STextDrawContext {
	STextDrawContext() {
		RtlSecureZeroMemory(this, sizeof(*this));
		m_proportional = true;
		m_sizeIn800x600 = true;
	}
	STextDrawContext(int colorOverride, float x, float y, float widthScale) {
		RtlSecureZeroMemory(this, sizeof(*this));
		m_proportional = true;
		m_colorOverride = colorOverride;
		m_widthScale = widthScale;
		m_size.x = x;
		m_size.y = y;
	}
	unsigned int m_fxIdx;
	Vec2 m_size;
	float m_widthScale;
	float m_clipX;
	float m_clipY;
	float m_clipWidth;
	float m_clipHeight;
	bool m_proportional;
	bool m_sizeIn800x600;
	bool m_clippingEnabled;
	bool m_framed;
	int m_colorOverride;
	int m_drawTextFlags;
};
struct SVF_P3F_C4B_T2F {
	SVF_P3F_C4B_T2F(float x, float y, float s, float t, DWORD c) {
		xyz = { x, y, 0.0f };
		color.dcolor = c;
		st = { s, t };
	}
	Vec3 xyz;
	union {
		DWORD dcolor;
		BYTE bcolor[4];
	}color;
	Vec2 st;
};
struct DECLSPEC_NOVTABLE IFFont {
	virtual void Function0() {};
	virtual void Function1() {};
	virtual void Function2() {};
	virtual bool Load(const char* pXMLFile) = 0;
	virtual void Function4() {};
	virtual void Function5() {};
	virtual void Function6() {};
	virtual void DrawString(float x, float y, const char* pStr, const bool asciiMultiLine, const STextDrawContext& ctx) = 0;
	virtual void DrawStringW(float x, float y, float z, const wchar_t* pStr, const bool asciiMultiLine, const STextDrawContext& ctx) = 0;
	virtual void Function9() {};
	virtual Vec2 GetTextSize(const char* pStr, const bool asciiMultiLine, const STextDrawContext& ctx) = 0;
	virtual Vec2 GetTextSizeW(const wchar_t* pStr, const bool asciiMultiLine, const STextDrawContext& ctx) = 0;
	virtual int GetTextLength(const char* szwMsg, const bool bASCIIMultiLine) = 0;
	virtual int GetTextLengthW(const wchar_t* szwMsg, const bool bASCIIMultiLine) = 0;
};
class ICryFont {
public:
	virtual ~ICryFont() {}
	virtual void Release() = 0;
	virtual IFFont* NewFont(char* pFontName) = 0;
	virtual IFFont* GetFont(const char* pFontName = ("warface")) const = 0;
};

struct SWorldToScreen
{
public:
	float toX;
	float toY;
	float toZ;
	float* outX;
	float* outY;
	float* outZ;
};


enum TextFlags {
	TEXT_RIGHT,
	TEXT_CENTERED_X,
	TEXT_CENTERED_Y,
	TEXT_CENTERED,
	TEXT_LEFT
};

enum EFRAMEWORKLISTENERPRIORITY
{
	FRAMEWORKLISTENERPRIORITY_DEFAULT,
	FRAMEWORKLISTENERPRIORITY_GAME,
	FRAMEWORKLISTENERPRIORITY_HUD,
	FRAMEWORKLISTENERPRIORITY_MENU
};

enum EUIDRAWHORIZONTAL {
	UIDRAWHORIZONTAL_LEFT,
	UIDRAWHORIZONTAL_CENTER,
	UIDRAWHORIZONTAL_RIGHT
};
enum EUIDRAWVERTICAL {
	UIDRAWVERTICAL_TOP,
	UIDRAWVERTICAL_CENTER,
	UIDRAWVERTICAL_BOTTOM
};

enum class EInputState
{
	eIS_Unknown = 0x0,
	eIS_Pressed = 0x1,
	eIS_Released = 0x2,
	eIS_Down = 0x4,
	eIS_Changed = 0x8,
	eIS_UI = 0x10,
};

enum class EDeviceId
{
	eDI_Keyboard = 0x0,
	eDI_Mouse = 0x1,
	eDI_XI = 0x2,
	eDI_Orbis = 0x3,
	eDI_Unknown = 0xFF,
};

enum class EKeyId : unsigned __int32
{
	eKI_Escape = 0x0,
	eKI_1 = 0x1,
	eKI_2 = 0x2,
	eKI_3 = 0x3,
	eKI_4 = 0x4,
	eKI_5 = 0x5,
	eKI_6 = 0x6,
	eKI_7 = 0x7,
	eKI_8 = 0x8,
	eKI_9 = 0x9,
	eKI_0 = 0xA,
	eKI_Minus = 0xB,
	eKI_Equals = 0xC,
	eKI_Backspace = 0xD,
	eKI_Tab = 0xE,
	eKI_Q = 0xF,
	eKI_W = 0x10,
	eKI_E = 0x11,
	eKI_R = 0x12,
	eKI_T = 0x13,
	eKI_Y = 0x14,
	eKI_U = 0x15,
	eKI_I = 0x16,
	eKI_O = 0x17,
	eKI_P = 0x18,
	eKI_LBracket = 0x19,
	eKI_RBracket = 0x1A,
	eKI_Enter = 0x1B,
	eKI_LCtrl = 0x1C,
	eKI_A = 0x1D,
	eKI_S = 0x1E,
	eKI_D = 0x1F,
	eKI_F = 0x20,
	eKI_G = 0x21,
	eKI_H = 0x22,
	eKI_J = 0x23,
	eKI_K = 0x24,
	eKI_L = 0x25,
	eKI_Semicolon = 0x26,
	eKI_Apostrophe = 0x27,
	eKI_Tilde = 0x28,
	eKI_LShift = 0x29,
	eKI_Backslash = 0x2A,
	eKI_Z = 0x2B,
	eKI_X = 0x2C,
	eKI_C = 0x2D,
	eKI_V = 0x2E,
	eKI_B = 0x2F,
	eKI_N = 0x30,
	eKI_M = 0x31,
	eKI_Comma = 0x32,
	eKI_Period = 0x33,
	eKI_Slash = 0x34,
	eKI_RShift = 0x35,
	eKI_NP_Multiply = 0x36,
	eKI_LAlt = 0x37,
	eKI_Space = 0x38,
	eKI_CapsLock = 0x39,
	eKI_F1 = 0x3A,
	eKI_F2 = 0x3B,
	eKI_F3 = 0x3C,
	eKI_F4 = 0x3D,
	eKI_F5 = 0x3E,
	eKI_F6 = 0x3F,
	eKI_F7 = 0x40,
	eKI_F8 = 0x41,
	eKI_F9 = 0x42,
	eKI_F10 = 0x43,
	eKI_NumLock = 0x44,
	eKI_ScrollLock = 0x45,
	eKI_NP_7 = 0x46,
	eKI_NP_8 = 0x47,
	eKI_NP_9 = 0x48,
	eKI_NP_Substract = 0x49,
	eKI_NP_4 = 0x4A,
	eKI_NP_5 = 0x4B,
	eKI_NP_6 = 0x4C,
	eKI_NP_Add = 0x4D,
	eKI_NP_1 = 0x4E,
	eKI_NP_2 = 0x4F,
	eKI_NP_3 = 0x50,
	eKI_NP_0 = 0x51,
	eKI_F11 = 0x52,
	eKI_F12 = 0x53,
	eKI_F13 = 0x54,
	eKI_F14 = 0x55,
	eKI_F15 = 0x56,
	eKI_Colon = 0x57,
	eKI_Underline = 0x58,
	eKI_NP_Enter = 0x59,
	eKI_RCtrl = 0x5A,
	eKI_NP_Period = 0x5B,
	eKI_NP_Divide = 0x5C,
	eKI_Print = 0x5D,
	eKI_RAlt = 0x5E,
	eKI_Pause = 0x5F,
	eKI_Home = 0x60,
	eKI_Up = 0x61,
	eKI_PgUp = 0x62,
	eKI_Left = 0x63,
	eKI_Right = 0x64,
	eKI_End = 0x65,
	eKI_Down = 0x66,
	eKI_PgDn = 0x67,
	eKI_Insert = 0x68,
	eKI_Delete = 0x69,
	eKI_LWin = 0x6A,
	eKI_RWin = 0x6B,
	eKI_Apps = 0x6C,
	eKI_OEM_102 = 0x6D,
	eKI_Mouse1 = 0x100,
	eKI_Mouse2 = 0x101,
	eKI_Mouse3 = 0x102,
	eKI_Mouse4 = 0x103,
	eKI_Mouse5 = 0x104,
	eKI_Mouse6 = 0x105,
	eKI_Mouse7 = 0x106,
	eKI_Mouse8 = 0x107,
	eKI_MouseWheelUp = 0x108,
	eKI_MouseWheelDown = 0x109,
	eKI_MouseX = 0x10A,
	eKI_MouseY = 0x10B,
	eKI_MouseZ = 0x10C,
	eKI_MouseLast = 0x10D,
	eKI_XI_DPadUp = 0x200,
	eKI_XI_DPadDown = 0x201,
	eKI_XI_DPadLeft = 0x202,
	eKI_XI_DPadRight = 0x203,
	eKI_XI_Start = 0x204,
	eKI_XI_Back = 0x205,
	eKI_XI_ThumbL = 0x206,
	eKI_XI_ThumbR = 0x207,
	eKI_XI_ShoulderL = 0x208,
	eKI_XI_ShoulderR = 0x209,
	eKI_XI_A = 0x20A,
	eKI_XI_B = 0x20B,
	eKI_XI_X = 0x20C,
	eKI_XI_Y = 0x20D,
	eKI_XI_TriggerL = 0x20E,
	eKI_XI_TriggerR = 0x20F,
	eKI_XI_ThumbLX = 0x210,
	eKI_XI_ThumbLY = 0x211,
	eKI_XI_ThumbLUp = 0x212,
	eKI_XI_ThumbLDown = 0x213,
	eKI_XI_ThumbLLeft = 0x214,
	eKI_XI_ThumbLRight = 0x215,
	eKI_XI_ThumbRX = 0x216,
	eKI_XI_ThumbRY = 0x217,
	eKI_XI_ThumbRUp = 0x218,
	eKI_XI_ThumbRDown = 0x219,
	eKI_XI_ThumbRLeft = 0x21A,
	eKI_XI_ThumbRRight = 0x21B,
	eKI_XI_TriggerLBtn = 0x21C,
	eKI_XI_TriggerRBtn = 0x21D,
	eKI_XI_Connect = 0x21E,
	eKI_XI_Disconnect = 0x21F,
	eKI_PS3_Select = 0x400,
	eKI_PS3_L3 = 0x401,
	eKI_PS3_R3 = 0x402,
	eKI_PS3_Start = 0x403,
	eKI_PS3_Up = 0x404,
	eKI_PS3_Right = 0x405,
	eKI_PS3_Down = 0x406,
	eKI_PS3_Left = 0x407,
	eKI_PS3_L2 = 0x408,
	eKI_PS3_R2 = 0x409,
	eKI_PS3_L1 = 0x40A,
	eKI_PS3_R1 = 0x40B,
	eKI_PS3_Triangle = 0x40C,
	eKI_PS3_Circle = 0x40D,
	eKI_PS3_Cross = 0x40E,
	eKI_PS3_Square = 0x40F,
	eKI_PS3_StickLX = 0x410,
	eKI_PS3_StickLY = 0x411,
	eKI_PS3_StickRX = 0x412,
	eKI_PS3_StickRY = 0x413,
	eKI_PS3_RotX = 0x414,
	eKI_PS3_RotY = 0x415,
	eKI_PS3_RotZ = 0x416,
	eKI_PS3_RotX_KeyL = 0x417,
	eKI_PS3_RotX_KeyR = 0x418,
	eKI_PS3_RotZ_KeyD = 0x419,
	eKI_PS3_RotZ_KeyU = 0x41A,
	eKI_Orbis_Options = 0x800,
	eKI_Orbis_L3 = 0x801,
	eKI_Orbis_R3 = 0x802,
	eKI_Orbis_Up = 0x803,
	eKI_Orbis_Right = 0x804,
	eKI_Orbis_Down = 0x805,
	eKI_Orbis_Left = 0x806,
	eKI_Orbis_L2 = 0x807,
	eKI_Orbis_R2 = 0x808,
	eKI_Orbis_L1 = 0x809,
	eKI_Orbis_R1 = 0x80A,
	eKI_Orbis_Triangle = 0x80B,
	eKI_Orbis_Circle = 0x80C,
	eKI_Orbis_Cross = 0x80D,
	eKI_Orbis_Square = 0x80E,
	eKI_Orbis_StickLX = 0x80F,
	eKI_Orbis_StickLY = 0x810,
	eKI_Orbis_StickRX = 0x811,
	eKI_Orbis_StickRY = 0x812,
	eKI_Orbis_RotX = 0x813,
	eKI_Orbis_RotY = 0x814,
	eKI_Orbis_RotZ = 0x815,
	eKI_Orbis_RotX_KeyL = 0x816,
	eKI_Orbis_RotX_KeyR = 0x817,
	eKI_Orbis_RotZ_KeyD = 0x818,
	eKI_Orbis_RotZ_KeyU = 0x819,
	eKI_Orbis_LeftTrigger = 0x81A,
	eKI_Orbis_RightTrigger = 0x81B,
	eKI_Orbis_StickLL = 0x81C,
	eKI_Orbis_StickLR = 0x81D,
	eKI_Orbis_StickLD = 0x81E,
	eKI_Orbis_StickLU = 0x81F,
	eKI_Orbis_Touch = 0x820,
	eKI_SYS_Commit = 0x1000,
	eKI_SYS_ConnectDevice = 0x1001,
	eKI_SYS_DisconnectDevice = 0x1002,
	eKI_Unknown = 0xFFFFFFFF,
};

enum EBone : uint64
{
	Bone_Pelvis = 1,
	Bone_Locomotion,
	Bone_Spine,
	Bone_L_Thigh,
	Bone_R_Thigh,
	Bone_Groin_Back,
	Bone_Groin_Front,
	Bone_Spine1,
	Bone_Spine2,
	Bone_Spine3,
	Bone_Wep_Law,
	Bone_Neck,
	Bone_Head,
	Bone_L_Clavicle,
	Bone_R_Clavicle,
	Bone_L_Eye,
	Bone_R_Eye,
	Bone_Camera,
	Bone_L_Eye_01,
	Bone_R_Eye_01,
	Bone_HNeck,
	Bone_Camera_01,
	Bone_HNeck_End,
	Bone_L_UpperArm,
	Bone_L_Forearm,
	Bone_L_Hand,
	Bone_L_ForeTwist,
	Bone_L_Finger0,
	Bone_L_Finger1,
	Bone_L_Finger2,
	Bone_L_Finger3,
	Bone_L_Finger4,
	Bone_Wep_Alt,
	Bone_L_Hand_Push,
	Bone_L_Finger01,
	Bone_L_Finger02,
	Bone_L_Finger11,
	Bone_L_Finger12,
	Bone_L_Finger21,
	Bone_L_Finger22,
	Bone_L_Finger31,
	Bone_L_Finger32,
	Bone_L_Finger41,
	Bone_L_Finger42,
	Bone_L_ForeTwist_1,
	Bone_L_ForeTwist_2,
	Bone_R_UpperArm,
	Bone_R_Forearm,
	Bone_R_Hand,
	Bone_R_ForeTwist,
	Bone_R_Finger0,
	Bone_R_Finger1,
	Bone_R_Finger2,
	Bone_R_Finger3,
	Bone_R_Finger4,
	Bone_WepBone,
	Bone_R_Hand_Push,
	Bone_R_Finger01,
	Bone_R_Finger02,
	Bone_R_Finger11,
	Bone_R_Finger12,
	Bone_R_Finger21,
	Bone_R_Finger22,
	Bone_R_Finger31,
	Bone_R_Finger32,
	Bone_R_Finger41,
	Bone_R_Finger42,
	Bone_R_ForeTwist_1,
	Bone_R_ForeTwist_2,
	Bone_L_Calf,
	Bone_L_Foot,
	Bone_L_Toe0,
	Bone_L_Heel,
	Bone_L_Heel01,
	Bone_L_Toe0Nub,
	Bone_L_Toe0Nub01,
	Bone_R_Calf,
	Bone_R_Foot,
	Bone_R_Toe0,
	Bone_R_Heel,
	Bone_R_Heel01,
	Bone_R_Toe0Nub,
	Bone_R_Toe0Nub01,
	Bone_Groin_Back_End,
	Bone_Groin_Front_End,
	Bone_Locator_Collider
};


template <class TVarType, typename TMaskType = uint32_t, TMaskType TMaxMaskValue = 0x7FFFFFFF>
class CMaskedVarT
{
public:
	CMaskedVarT() : m_mask(0) { }
	CMaskedVarT(const TVarType v) { Set(v); }
	operator TVarType () const { return this->Get(); }
	CMaskedVarT& operator += (const TVarType rhs)
	{
		TVarType  v = this->Get();
		v += rhs;
		this->Set(v);
		return (*this);
	}
	CMaskedVarT& operator -= (const TVarType rhs)
	{
		TVarType  v = this->Get();
		v -= rhs;
		this->Set(v);
		return (*this);
	}
	CMaskedVarT& operator *= (const TVarType rhs)
	{
		TVarType  v = this->Get();
		v *= rhs;
		this->Set(v);
		return (*this);
	}
	CMaskedVarT& operator /= (const TVarType rhs)
	{
		TVarType  v = this->Get();
		v /= rhs;
		this->Set(v);
		return (*this);
	}
	CMaskedVarT& operator = (const TVarType rhs)
	{
		Set(rhs);
		return (*this);
	}
private:
	void Set(const TVarType v)
	{
		UVal		w;
		w.asVarType = v;
		m_val.asMaskType = (w.asMaskType ^ m_mask);
	}
	TVarType Get() const
	{
		UVal  w;
		w.asMaskType = 0;
		if (m_mask > 0) w.asMaskType = (m_val.asMaskType ^ m_mask);
		return w.asVarType;
	}
	union UVal
	{
		TVarType  asVarType;
		TMaskType  asMaskType;
	};
	UVal  m_val;
	TMaskType  m_mask;
};

using TMaskedFloat = CMaskedVarT<float>;