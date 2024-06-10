#pragma once
#include <memory>
#include "camera.h"
#include "SoftRenderer.h"
#include "vertex.h"
#include <vector>
#include <windowsx.h>
#include "shader.h"
#include "light.h"
#include <ctime>




namespace SoftRenderer
{
	int g_width = 0;
	int g_height = 0;

	HDC g_tempDC = nullptr;
	HBITMAP g_tempBm = nullptr;
	HBITMAP g_oldBm = nullptr;
	unsigned int* g_frameBuff = nullptr;
	std::shared_ptr<float[]> g_depthBuff = nullptr;

	unsigned int bgColor = ((0 << 16) | (0 << 8) | 0);//背景色
	unsigned int defaultColor = ((0 << 16) | (255 << 8) | 0);//默认颜色

	enum class RenderMode
	{
		RENDER_WIREFRAME,
		RENDER_COLOR,
	};
	RenderMode g_renderMode = RenderMode::RENDER_COLOR;


	point lookfrom = { 15.0f,5.0f,30.0f,1.0f };
	point lookat = { 0.0,0.0,0.0,1.0 };
	vec4 up = { 0.0f,1.0f,0.0f,0.0f };

	Camera camera(lookfrom, lookat, up);

	point light_position = { 5.25f,5.25f,-5.25f,1.0f };
	//vec4 light_dir = -light_position;
	Light light( light_position );

	std::vector<vertex> cubes;
	void initCubes(std::vector<vertex>& cubes);

	

	std::vector<vertex> light_cube = {
		{{5.0f,5.0f,-2.0f,1.0f},{1.0f,1.0f,1.0f,0.0f}},
		{{5.5f,5.0f,-2.0f,1.0f},{1.0f,1.0f,1.0f,0.0f}},
		{{5.5f,5.5f,-2.0f,1.0f},{1.0f,1.0f,1.0f,0.0f}},
		{{5.0f,5.5f,-2.0f,1.0f},{1.0f,1.0f,1.0f,0.0f}},

		{{5.0f,5.0f,-2.5f,1.0f},{1.0f,1.0f,1.0f,0.0f}},
		{{5.5f,5.0f,-2.5f,1.0f},{1.0f,1.0f,1.0f,0.0f}},
		{{5.5f,5.5f,-2.5f,1.0f},{1.0f,1.0f,1.0f,0.0f}},
		{{5.0f,5.5f,-2.5f,1.0f},{1.0f,1.0f,1.0f,0.0f}}
	};

	std::vector<vertex> floor = {
		{{-8.0f,-1.5f,8.0f,1.0f},{1.0f,1.0f,1.0f,0.0f}},
		{{8.0f,-1.5f,8.0f,1.0f},{1.0f,1.0f,1.0f,0.0f}},
		{{8.0f,-1.0f,8.0f,1.0f},{1.0f,1.0f,1.0f,0.0f}},
		{{-8.0f,-1.0f,8.0f,1.0f},{1.0f,1.0f,1.0f,0.0f}},

		{{-8.0f,-1.5f,-8.0f,1.0f},{1.0f,1.0f,1.0f,0.0f}},
		{{8.0f,-1.5f,-8.0f,1.0f},{1.0f,1.0f,1.0f,0.0f}},
		{{8.0f,-1.0f,-8.0f,1.0f},{1.0f,1.0f,1.0f,0.0f}},
		{{-8.0f,-1.0f,-8.0f,1.0f},{1.0f,1.0f,1.0f,0.0f}}
	};


	Shader shader;

	void initRenderer(int w, int h, HWND hwnd);
	void update(HWND hwnd);

	void clearBuffer();
	void shutDown();

	void onMouseMessage(UINT message, WPARAM wParam, LPARAM lParam);



	//绘制立方体
	void drawCube(std::vector<vertex>& cubes);
	//绘制平面
	void drawPlane(int lt, int rt, int rb, int lb,std::vector<vertex>& cube);
	//绘制三角形
	void drawPrimitive(const vertex& a, const vertex& b, const vertex& c);
	//扫描线绘制线框三角形
	void drawPrimitiveScanLine(const vertex& a, const vertex& b, const vertex& c);
	//绘制线
	void drawLine(int x1, int y1, int x2, int y2, unsigned int color);
	//绘制点
	void drawPixel(int x, int y, unsigned int color);
	//Cvv裁剪，目前仅能把三角形全部去除
	bool checkCvv(const vec4& v);

	vec4 barycentric(const vec4& a, const vec4& b, const vec4& c, const vec4& p);


}

void SoftRenderer::initRenderer(int w,int h,HWND hwnd) {
	g_width = w;
	g_height = h;

	//创建屏幕缓冲
	HDC hdc = GetDC(hwnd);
	g_tempDC = CreateCompatibleDC(hdc);
	ReleaseDC(hwnd, hdc);
	//创建DC的bitmap缓冲，32位色
	BITMAPINFO bi = { {sizeof(BITMAPINFOHEADER),w,-h,1,32,BI_RGB,(DWORD)w * h * 4,0,0,0,0} };
	g_tempBm = CreateDIBSection(g_tempDC, &bi, DIB_RGB_COLORS, (void**)&g_frameBuff, 0, 0);
	g_oldBm = (HBITMAP)SelectObject(g_tempDC, g_tempBm);
	//创建深度缓冲
	g_depthBuff.reset(new float[w * h]);
	clearBuffer();

	camera.setPerspective(PI * 0.25f, (float)w / (float)h, 1.0f, 200.0f);//fov=45°


}


void SoftRenderer::update(HWND hwnd)
{

	//首先清理屏幕缓冲
	clearBuffer();

	clock_t startT = clock();
	initCubes(cubes);
	drawCube(cubes);
	

	//drawCube(light_cube);
	drawCube(floor);

	HDC hdc = GetDC(hwnd);
	//显示FPS

	TCHAR  fps[50] = {0};

	BitBlt(hdc, 0, 0, g_width, g_height, g_tempDC, 0, 0, SRCCOPY);

	clock_t endT = clock();

	float endtime = (float)(endT - startT) / CLOCKS_PER_SEC;
	float fps_num = 60.0f / endtime;

	swprintf(fps, 50, L"FPS: %f", fps_num);
	SetTextColor(hdc,(0 << 16) | (0 << 8) | 255);
	TextOut(hdc, 5, 5, fps, 12);

	ReleaseDC(hwnd, hdc);
}

void SoftRenderer::clearBuffer()
{
	for (int row = 0; row < g_height; row++)
	{
		for (int col = 0; col < g_width; col++)
		{
			int idx = row * g_width + col;
			g_frameBuff[idx] = bgColor;//为默认背景色

			g_depthBuff[idx] = 1.0f;//深度为默认1.0；
		}
	}
}

void SoftRenderer::shutDown() {
	if (g_tempDC)
	{
		if (g_oldBm)
		{
			SelectObject(g_tempDC, g_oldBm);
			g_oldBm = nullptr;
		}
		
		DeleteDC(g_tempDC);
		g_tempDC = nullptr;
	}

	if (g_tempBm)
	{
		DeleteObject(g_tempBm);
		g_tempBm = nullptr;
	}
}

void SoftRenderer::onMouseMessage(UINT message, WPARAM wParam, LPARAM lParam)
{
	static bool isPressed = false;
	static LPARAM lpCur = 0;
	switch (message)
	{
	case WM_MOUSEWHEEL:
		camera.zoom(GET_WHEEL_DELTA_WPARAM(wParam));
		break;
	case WM_LBUTTONDOWN:
		isPressed = true;
		lpCur = lParam;
		break;
	case WM_LBUTTONUP:
		isPressed = false;
		break;
	case WM_MOUSEMOVE:
		if ( wParam && MK_LBUTTON)
		{
			short xMove ;
			short yMove ;
			if (lpCur == 0)
			{
				xMove = 0;
				yMove = 0;
			}
			else
			{
				xMove = GET_X_LPARAM(lParam) - GET_X_LPARAM(lpCur);
				yMove = GET_Y_LPARAM(lParam) - GET_Y_LPARAM(lpCur);
			}
			
			float sensitivity = 0.1;
			camera.circle(xMove, yMove,sensitivity);

			lpCur = lParam;
		}
		break;
	case WM_KEYDOWN:
		if (wParam == 0x52)//按下R键，重置相机
			camera.reset();
		if (wParam == VK_ESCAPE)//按下ESC，退出窗口
			PostQuitMessage(0);
		if (wParam == 0x57)//按下W键，相机前移
			camera.front();
		if (wParam == 0x41)//按下A键，相机左移
			camera.left();
		if (wParam == 0x53)//按下S键，相机后移
			camera.back();
		if (wParam == 0x44)//按下D键，相机右移
			camera.right();
		if (wParam == VK_F1)
		{
			if (g_renderMode == RenderMode::RENDER_COLOR)
				g_renderMode = RenderMode::RENDER_WIREFRAME;
			else
				g_renderMode = RenderMode::RENDER_COLOR;
		}


	}

}

void SoftRenderer::initCubes(std::vector<vertex>& cubes) {
	vertex vertex_1;
	vertex_1.coord = { -1.0f, +1.0f, 1.0f, 1.0f };
	vertex_1.color = { 1.0f, 0.0f, 0.0f, 0.0f };
	cubes.push_back(vertex_1);
	vertex vertex_2;
	vertex_2.coord = { +1.0f, +1.0f, 1.0f, 1.0f };
	vertex_2.color = { 0.0f, 1.0f, 0.0f, 0.0f };
	cubes.push_back(vertex_2);
	vertex vertex_3;
	vertex_3.coord = { +1.0f, -1.0f, 1.0f, 1.0f };
	vertex_3.color = { 0.0f, 0.0f, 1.0f, 0.0f };
	cubes.push_back(vertex_3);
	vertex vertex_4;
	vertex_4.coord = { -1.0f, -1.0f, 1.0f, 1.0f };
	vertex_4.color = { 1.0f, 0.0f, 0.0f, 0.0f };
	cubes.push_back(vertex_4);
	vertex vertex_5;
	vertex_5.coord = { -1.0f, +1.0f, -1.0f, 1.0f };
	vertex_5.color = { 0.0f, 1.0f, 0.0f, 0.0f };
	cubes.push_back(vertex_5);
	vertex vertex_6;
	vertex_6.coord = { +1.0f, +1.0f, -1.0f, 1.0f };
	vertex_6.color = { 0.0f, 0.0f, 1.0f, 0.0f };
	cubes.push_back(vertex_6);
	vertex vertex_7;
	vertex_7.coord = { +1.0f, -1.0f, -1.0f, 1.0f };
	vertex_7.color = { 1.0f, 0.0f, 0.0f, 0.0f };
	cubes.push_back(vertex_7);
	vertex vertex_8;
	vertex_8.coord = { -1.0f, -1.0f, -1.0f, 1.0f };
	vertex_8.color = { 0.0f, 1.0f, 0.0f, 0.0f };
	cubes.push_back(vertex_8);

}



//绘制一个立方体
void SoftRenderer::drawCube(std::vector<vertex>& cube) {
	
	//计算法线，在绘制平面时把法线数据加入到点中。因为立方体的三角形面数太少，因此每个点需要分开来计算法线
	vec4 normal = (cube[0].coord - cube[1].coord).cross(cube[0].coord - cube[2].coord);
	normal = normal.dot(cube[0].coord - cube[5].coord) > 0 ? normal : - normal;
	normal = normal.normalize();
	cube[0].normal = normal; cube[1].normal = normal; cube[2].normal = normal; cube[3].normal = normal;
	drawPlane(0, 1, 2, 3,cube);  // 正面

	normal = (cube[1].coord - cube[5].coord).cross(cube[1].coord - cube[6].coord);
	normal = normal.dot(cube[1].coord - cube[0].coord) > 0 ? normal : -normal;
	normal = normal.normalize();
	cube[1].normal = normal; cube[5].normal = normal; cube[6].normal = normal; cube[2].normal = normal;
	drawPlane(1, 5, 6, 2,cube);  // 右面

	normal = (cube[4].coord - cube[0].coord).cross(cube[4].coord - cube[3].coord);
	normal = normal.dot(cube[4].coord - cube[1].coord) > 0 ? normal : -normal;
	normal = normal.normalize();
	cube[4].normal = normal; cube[0].normal= normal; cube[3].normal = normal; cube[7].normal = normal;
	drawPlane(4, 0, 3, 7,cube);  // 左面

	normal = (cube[4].coord - cube[5].coord).cross(cube[4].coord - cube[1].coord);
	normal = normal.dot(cube[4].coord - cube[2].coord) > 0 ? normal : -normal;
	normal = normal.normalize();
	cube[4].normal = normal; cube[5].normal = normal; cube[1].normal = normal; cube[0].normal = normal;
	drawPlane(4, 5, 1, 0,cube);  // 上面

	normal = (cube[3].coord - cube[2].coord).cross(cube[3].coord - cube[6].coord);
	normal = normal.dot(cube[3].coord - cube[0].coord) > 0 ? normal : -normal;
	normal = normal.normalize();
	cube[3].normal = normal; cube[2].normal = normal; cube[6].normal = normal; cube[7].normal = normal;
	drawPlane(3, 2, 6, 7,cube);  // 下面

	normal = (cube[5].coord - cube[4].coord).cross(cube[5].coord - cube[7].coord);
	normal = normal.dot(cube[5].coord - cube[0].coord) > 0 ? normal : -normal;
	normal = normal.normalize();
	cube[5].normal = normal; cube[4].normal = normal; cube[7].normal = normal; cube[6].normal = normal;
	drawPlane(5, 4, 7, 6,cube);  // 后面

}
//绘制一个平面，也即两个三角形
void SoftRenderer::drawPlane(int lt,int rt,int rb,int lb,std::vector<vertex>& cube) {
	
	drawPrimitive(cube[lt], cube[rt], cube[rb]);
	drawPrimitive(cube[lt], cube[rb], cube[lb]);
}

//绘制一个三角形
void SoftRenderer::drawPrimitive(const vertex& a, const vertex& b, const vertex& c) {
	matrix4f m = camera.WorldToPerspective;
	vec4 p1 = m * a.coord; if (checkCvv(p1)) return;
	vec4 p2 = m * b.coord; if (checkCvv(p2)) return;
	vec4 p3 = m * c.coord; if (checkCvv(p3)) return;

	Math::perspectiveDivide(p1);//透视除法
	Math::perspectiveDivide(p2);
	Math::perspectiveDivide(p3);

	//转换到屏幕空间
	Math::transformScreen(p1, g_width, g_height);
	Math::transformScreen(p2, g_width, g_height);
	Math::transformScreen(p3, g_width, g_height);

	//同理，法线也需要转换，乘以逆转置矩阵
	matrix4f inverse = m.getInverse();
	inverse.transpose();
	vec4 p1_normal =  a.normal;
	vec4 p2_normal =  b.normal;
	vec4 p3_normal =  c.normal;
	
	//Math::perspectiveDivide(p1_normal);
	//Math::perspectiveDivide(p2_normal);
	//Math::perspectiveDivide(p3_normal);


	//绘制线框
	if (g_renderMode == RenderMode::RENDER_WIREFRAME)
	{
		int x1 = (int)(p1.x + 0.5f), x2 = (int)(p2.x + 0.5f), x3 = (int)(p3.x + 0.5f);
		int y1 = (int)(p1.y + 0.5f), y2 = (int)(p2.y + 0.5f), y3 = (int)(p3.y + 0.5f);
		drawLine(x1, y1, x2, y2, defaultColor);
		drawLine(x2, y2, x3, y3, defaultColor);
		drawLine(x1, y1, x3, y3, defaultColor);
	}
	else
	{
		drawPrimitiveScanLine({ p1,a.color,p1_normal.normalize() }, { p2,b.color,p2_normal.normalize() }, { p3,c.color,p3_normal.normalize() });
	}

}

void SoftRenderer::drawPrimitiveScanLine(const vertex& a, const vertex& b, const vertex& c)
{
	float xl = a.coord.x; if (b.coord.x < xl) xl = b.coord.x; if (c.coord.x < xl) xl = c.coord.x;
	float xr = a.coord.x; if (b.coord.x > xr) xr = b.coord.x; if (c.coord.x > xr) xr = c.coord.x;
	float yt = a.coord.y; if (b.coord.y < yt) yt = b.coord.y; if (c.coord.y < yt) yt = c.coord.y;
	float yb = a.coord.y; if (b.coord.y > yb) yb = b.coord.y; if (c.coord.y > yb) yb = c.coord.y;

	int xMin = (int)(xl + 0.5f), xMax = (int)(xr + 0.5f), yMin = (int)(yt + 0.5f), yMax = (int)(yb + 0.5f);
	for (int x = xMin; x <= xMax; ++x)
	{
		for (int y = yMin; y <= yMax; ++y)
		{
			// 计算重心坐标
			vec4 ret = barycentric(a.coord, b.coord, c.coord, { (float)x, (float)y, 0.0f, 0.0f });
			vec4 normal;
			if (ret.x < 0 || ret.y < 0 || ret.z < 0) continue;
			unsigned int colorR = (unsigned int)((a.color.x * ret.x + b.color.x * ret.y + c.color.x * ret.z) * 255);
			unsigned int colorG = (unsigned int)((a.color.y * ret.x + b.color.y * ret.y + c.color.y * ret.z) * 255);
			unsigned int colorB = (unsigned int)((a.color.z * ret.x + b.color.z * ret.y + c.color.z * ret.z) * 255);
			normal.x = (a.normal.x * ret.x + b.normal.x * ret.y + c.normal.x * ret.z);
			normal.y = (a.normal.y * ret.x + b.normal.y * ret.y + c.normal.y * ret.z);
			normal.z = (a.normal.z * ret.x + b.normal.z * ret.y + c.normal.z * ret.z);
			float depth = (a.coord.z * ret.x + b.coord.z * ret.y + c.coord.z * ret.z);
			if (g_depthBuff[x + y * g_width] < depth)continue;//比较深度
			g_depthBuff[x + y * g_width] = depth;

			//vec4 light_pos = camera.WorldToPerspective * light._pos;
			//vec4 camera_pos = camera.WorldToPerspective * camera._pos;
			//Math::perspectiveDivide(light_pos);
			//Math::perspectiveDivide(camera_pos);
			vec4 coord;
			coord.x = ((a.coord.x * ret.x + b.coord.x * ret.y + c.coord.x * ret.z));
			coord.y = ((a.coord.y * ret.x + b.coord.y * ret.y + c.coord.y * ret.z) );
			coord.z = ((a.coord.z * ret.x + b.coord.z * ret.y + c.coord.z * ret.z) );

			float color = shader.shade(ret, normal, light._pos, camera._pos);
			colorR *= color; if (colorR > 255) colorR = 255;
			colorG *= color; if (colorG > 255) colorG = 255;
			colorB *= color; if (colorB > 255) colorB = 255;
			//法线没有问题
			unsigned int normal_x = normal.x * 255;
			unsigned int normal_y = normal.y * 255;
			unsigned int normal_z = normal.z * 255;
			drawPixel(x, y, (colorR << 16) | (colorG << 8) | colorB);
		}
	}
}

void SoftRenderer::drawLine(int x1, int y1, int x2, int y2, unsigned int color)
{
	if (x1 == x2 && y1 == y2) drawPixel(x1, y1, color);

	else if (x1 == x2)
	{
		if (y1 > y2) std::swap(y1, y2);
		for (int y = y1; y <= y2; y++)
			drawPixel(x1, y, color);
	}

	else if (y1 == y2)
	{
		if (x1 > x2) std::swap(x1, x2);
		for (int x = x1; x <= x2; x++)
			drawPixel(x, y1, color);
	}

	else
	{
		//bresenham 算法
		int dx = std::abs(x1 - x2);
		int dy = std::abs(y1 - y2);
		int diff = 0;
		if (dx > dy)
		{
			//采用判断是否高于x=y直线，可以免去除法操作
			if (x1 > x2) std::swap(x1, x2), std::swap(y1, y2);
			for (int x = x1, y = y1; x < x2; ++x)
			{
				drawPixel(x, y, color);
				diff += dy;
				if (diff >= dx)
				{
					diff -= dx;
					y += (y1 < y2) ? 1 : -1;
				}
			}
			drawPixel(x2, y2, color);
		}
		else
		{
			if (y1 > y2) std::swap(x1, x2), std::swap(y1, y2);
			for (int y = y1, x = x1; y < y2; ++y)
			{
				drawPixel(x, y, color);
				diff += dx;
				if (diff >= dy)
				{
					diff -= dy;
					x += (x1 < x2) ? 1 : -1;
				}
			}
			drawPixel(x2, y2, color);
		}
	}

}

void SoftRenderer::drawPixel(int x, int y, unsigned int color)
{
	if (x < 0 || x >= g_width || y < 0 || y >= g_height) return;

	int idx = y * g_width + x;
	g_frameBuff[idx] = color;
}

//判断是否在显示器区域之外
bool SoftRenderer::checkCvv(const vec4& v)
{
	if (v.z < 0.0f)return true;
	if (v.z > v.w) return true;
	if (v.x < -v.w) return true;
	if (v.x > v.w) return true;
	if (v.y < -v.w)return true;
	if (v.y > v.w)return true;
	return false;
}

vec4 SoftRenderer::barycentric(const vec4& a, const vec4& b, const vec4& c, const vec4& p) 
{
	vec4 v1 = { c.x - a.x,b.x - a.x,a.x - p.x };
	vec4 v2 = { c.y - a.y,b.y - a.y,a.y - p.y };

	vec4 u = v1.cross(v2);
	if (std::abs(u.z) > 1e-2)
		return { 1.f - (u.x + u.y) / u.z,u.y / u.z,u.x / u.z };
	return { -1,1,1,0 };
}






