#include "graphics.h"
#include <iostream>
#include <fstream>
using namespace std;

struct point {
	int x, y;
};

void mystar(point a, point b)
{
	line(a.x, a.y + (b.y - a.y) / 2.5, b.x, a.y + (b.y - a.y) / 2.5);
	line(a.x + (b.x - a.x) / 4, b.y, a.x + (b.x - a.x) / 2, a.y);
	line(a.x + (b.x - a.x) / 2, a.y, a.x + 3 * (b.x - a.x) / 4, b.y);
	line(a.x + (b.x - a.x) / 4, b.y, b.x, a.y + (b.y - a.y) / 2.5);
	line(a.x + 3 * (b.x - a.x) / 4, b.y, a.x, a.y + (b.y - a.y) / 2.5);
}

void myrectangle(int left, int top, int right, int bottom)
{
	line(left, top, right, top);
	line(right, top, right, bottom);
	line(right, bottom, left, bottom);
	line(left, bottom, left, top);
}

void triangle(int x1, int y1, int x2, int y2, int x3, int y3)
{
	line(x1, y1, x2, y2);
	line(x2, y2, x3, y3);
	line(x3, y3, x1, y1);
}

void dottedline(int x1, int y1, int x2, int y2)
{
	setlinestyle(1, 0x3333, 1);
	line(x1, y1, x2, y2);
	setlinestyle(0, 0xFFFF, 1);
}

void myfilledrect(int color, int x1, int y1, int x2, int y2)
{
	setcolor(color);
	myrectangle(x1, y1, x2, y2);
	setfillstyle(SOLID_FILL, color);
	floodfill(x1 + 1, y1 + 1, color);
	setcolor(15);
	setfillstyle(EMPTY_FILL, 0);
}

void mypoly(int n, point *pts)
{
	int i;
	for (i = 0; i < n - 1; i++)
	{
		line(pts[i].x, pts[i].y, pts[i + 1].x, pts[i + 1].y);
	}

}

void panel()
{
	myrectangle(100, 0, 200, 100);                          //confines of colour palette
	myfilledrect(14, 100, 0, 150, 50);                      //yellow
	myfilledrect(COLOR(178,34,34), 150, 0, 200, 50);        //red
	myfilledrect(0, 100, 50, 150, 100);                     //black
	myfilledrect(15, 150, 50, 200, 100);                    //white
	myfilledrect(COLOR(0,128,128), 100, 100, 150, 150);     //blue
	myfilledrect(COLOR(0,128,0), 150, 100, 200, 150);       //green
	myfilledrect(COLOR(255, 0, 255), 100, 150, 150, 200);   //pink
	myfilledrect(COLOR(255,69,0), 150, 150, 200, 200);      //orange

	myrectangle(0, 0, 100, 100);  //confines of rect
	myrectangle(20, 40, 80, 60);
	moveto(20, 65);
	outtext("rectangle");
	myrectangle(0, 100, 100, 200);  //confines of triangle
	triangle(20, 160, 50, 140, 80, 160);
	moveto(25, 165);
	outtext("triangle");
	myrectangle(0, 200, 100, 300);  //confines of circle
	circle(50, 235, 25);
	moveto(25, 265);
	outtext("circle");
	myrectangle(0, 300, 100, 400);  //confines of solid line
	line(20, 350, 80, 350);
	moveto(20, 365);
	outtext("solid line");
	myrectangle(0, 400, 100, 500);  //confines of dotted line
	dottedline(20, 450, 80, 450);
	moveto(15, 465);
	outtext("dotted line");
	myrectangle(0, 500, 100, 600);  //confines of text
	moveto(40, 540);
	outtext("text");
	myrectangle(100, 200, 200, 300);  //confines of poly
	moveto(120, 240);
	outtext("polygon");
	myrectangle(100, 300, 200, 400);  //confines of curve
	moveto(120, 340);
	outtext("curve");
	myrectangle(100, 400, 200, 500);  //confines of eraser
	moveto(120, 440);
	outtext("eraser");
	myrectangle(100, 500, 200, 600);  //confines of bucket
	moveto(120, 540);
	outtext("bucket");

	myrectangle(200, 0, 300, 50);  //confines of save
	moveto(230, 18);
	outtext("SAVE");

}


ofstream fout("shapes.txt");

class shape {
protected:
	point * points;
	int color;
	int numpoints;
public:
	shape(point* temp_points, int temp_color, int temp_numpoints)
	{
		color = temp_color;
		numpoints = temp_numpoints;
		points = new point[numpoints];
		for (int i = 0; i < numpoints; i++)
		{
			points[i] = temp_points[i];
		}
	}
	virtual void draw(int c) = 0;
	virtual bool containsPoint(point p) = 0;
	virtual void save()
	{

		fout << numpoints << endl;
		for (int i = 0; i < numpoints; i++)
			fout << points[i].x << " " << points[i].y << " ";
		fout << color << endl;

	}
	//virtual~shape()
	//{
	//	delete points;
	//}
};

class Text :public shape 
{
	char *text;
public:
	Text(point* temp_points, int temp_color, int temp_numpoints, char *temp_text)
		:shape(temp_points, temp_color, temp_numpoints)
	{
		text = new char[strlen(temp_text) + 1];
		for (int i = 0; i < strlen(temp_text) + 1; i++)
			text[i] = temp_text[i];
	}
	void draw(int c)
	{
		setcolor(BLACK);
		moveto(points[0].x, points[0].y);
		outtext(text);
	}
	bool containsPoint(point p)
	{
		return true;
	}

	void save()
	{
		shape::save();
		fout << text << endl;
	}
};

class polygon :public shape
{
protected:
	int fillcolor;
public:
	polygon(point *temp_points, int temp_color, int temp_numpoints, int temp_fillcolor)
		:shape(temp_points, temp_color, temp_numpoints), fillcolor(temp_fillcolor)
	{

	}
	virtual void draw(int c) = 0;
	virtual bool containsPoint(point p) = 0;
};

class Rect :public polygon
{
public:
	Rect(point *temp_points, int temp_color, int temp_numpoints, int temp_fillcolor)
		:polygon(temp_points, temp_color, temp_numpoints, temp_fillcolor)
	{
	}
	void draw(int c)
	{
		if (c == 1)
			setcolor(color);
		else if (c == 0)
			setcolor(BLACK);
		myrectangle(points[0].x, points[0].y, points[1].x, points[1].y);
	}
	bool containsPoint(point p)
	{
		if (p.x >= points[0].x && p.x <= points[1].x && p.y < points[1].y && p.y > points[0].y)
			return true;
		else if (p.x >= points[1].x && p.x <= points[0].x && p.y < points[0].y && p.y > points[1].y)
			return true;
		else
			return false;
	}
	void save()
	{
		shape::save();
	}
};

class Triangle :public polygon
{
public:
	Triangle(point *temp_points, int temp_color, int temp_numpoints, int temp_fillcolor)
		:polygon(temp_points, temp_color, temp_numpoints, temp_fillcolor)
	{
	}
	void draw(int c)
	{
		if (c == 1)
			setcolor(color);
		else if (c == 0)
			setcolor(BLACK);
		triangle(points[0].x, points[0].y, points[1].x, points[1].y, points[2].x, points[2].y);
	}
	bool containsPoint(point p)
	{

		return true;
	}
	void save()
	{
		shape::save();
	}
};

class userpoly :public polygon
{
public:
	userpoly(point *temp_points, int temp_color, int temp_numpoints, int temp_fillcolor)
		:polygon(temp_points, temp_color, temp_numpoints, temp_fillcolor)
	{

	}
	void draw(int c)
	{
		if (c == 1)
			setcolor(color);
		else if (c == 0)
			setcolor(BLACK);
		mypoly(numpoints, points);

	}
	bool containsPoint(point p)
	{
		return true;
	}
	void save()
	{
		shape::save();
	}
};

class star :public polygon
{
public:
	star(point *temp_points, int temp_color, int temp_numpoints, int temp_fillcolor)
		:polygon(temp_points, temp_color, temp_numpoints, temp_fillcolor)
	{

	}

	void draw(int c)
	{
		if (c == 1)
			setcolor(color);
		else if (c == 0)
			setcolor(BLACK);
		mystar(points[0], points[1]);
	}

	bool containsPoint(point p)
	{
		return true;
	}
	void save()
	{
		shape::save();
	}
};

class Circle :public shape
{
	double radius;
public:
	Circle(point *temp_points, int temp_color, int temp_numpoints, double temp_radius)
		:shape(temp_points, temp_color, temp_numpoints), radius(temp_radius)
	{
	}

	void draw(int c)
	{
		if (c == 1)
			setcolor(color);
		else if (c == 0)
			setcolor(BLACK);
		circle(points[0].x, points[0].y, radius);
	}
	bool containsPoint(point p)
	{
		if (sqrt((p.x - points[0].x)*(p.x - points[0].x) + (p.y - points[0].y)*(p.y - points[0].y)) <= radius)
			return true;
		else
			return false;

	}
	void save()
	{
		shape::save();
		fout << radius << endl;
	}
};

class openshape :public shape
{
protected:
	int style; // style is 1 for solid line and 2 for dotted line
public:
	openshape(point *temp_points, int temp_color, int temp_numpoints, int temp_style)
		:shape(temp_points, temp_color, temp_numpoints), style(temp_style)
	{
	}
	virtual void draw(int c) = 0;
	virtual bool containsPoint(point p) = 0;
};

class Line :public openshape
{
public:
	Line (point *temp_points, int temp_color, int temp_numpoints, int temp_style)
		:openshape(temp_points, temp_color, temp_numpoints, temp_style)
	{

	}
	void draw(int c)
	{
		if (c == 1)
			setcolor(color);
		else if (c == 0)
			setcolor(BLACK);
		if (style == 1)
			line(points[0].x, points[0].y, points[1].x, points[1].y);
		else 
			dottedline(points[0].x, points[0].y, points[1].x, points[1].y);
	}
	bool containsPoint(point p)
	{
		if ((points[1].y - points[0].y) / (points[1].x - points[0].x) == (points[1].y - p.y) / (points[1].x - p.x))
			return true;
		else
			return false;
	}
	void save()
	{
		shape::save();
	}
};

class curve :public openshape
{
public:
	curve(point *temp_points, int temp_color, int temp_numpoints, int temp_style)
		:openshape(temp_points, temp_color, temp_numpoints, temp_style)
	{

	}
	void draw(int c)
	{
		if (c == 1)
			setcolor(color);
		else if (c == 0)
			setcolor(BLACK);
		mypoly(numpoints, points);
	}
	bool containsPoint(point p)
	{
		return true;
	}
	void save()
	{
		shape::save();
	}
};

shape ** minimizeArray(shape** arr, int size, int cap)
{
	size = size + 1;
	if (size * 2 < cap)
	{
		shape**temp = new shape*[size * 2];
		for (int i = 0; i < size; i++)
			temp[i] = arr[i];
		return temp;
	}
	else
		return arr;
}

shape** increaseArray(shape**arr, int size, int &cap) 
{
	if (size == cap)
	{
		shape** temp = new shape*[size * 2];
		cap = size * 2;
		for (int i = 0; i < cap; i++)
		{
			temp[i] = arr[i];
		}
		return temp;
	}
	else 
		return arr;
}



int whereismouse(int x, int y, int &color, shape** allshapes, int &counter, int &capacity)
{
	int numpoints;
	allshapes = minimizeArray(allshapes, counter, capacity);
	if (x <= 100 && x >= 0 && y <= 100 && y >= 0)  //is rect?
	{
		int x1, y1, x2, y2;
		numpoints = 2;
		while (!ismouseclick(WM_LBUTTONDOWN))
		{
			delay(500);
		}
		getmouseclick(WM_LBUTTONDOWN, x1, y1);
		while (!ismouseclick(WM_LBUTTONDOWN))
		{
			delay(500);
		}
		getmouseclick(WM_LBUTTONDOWN, x2, y2);

		point *points = new point[numpoints];
		points[0].x = x1;
		points[0].y = y1;
		points[1].x = x2;
		points[1].y = y2;

		allshapes[counter] = new Rect(points, color, numpoints, 10);
		allshapes[counter]->draw(1);
		counter++;
		allshapes = increaseArray(allshapes, counter, capacity);

	}
	if (x <= 100 && x >= 0 && y <= 200 && y >= 100)  // is traingle?
	{
		numpoints = 3;
		int x1, x2, x3, y1, y2, y3;
		while (!ismouseclick(WM_LBUTTONDOWN))
		{
			delay(500);
		}
		getmouseclick(WM_LBUTTONDOWN, x1, y1);
		while (!ismouseclick(WM_LBUTTONDOWN))
		{
			delay(500);
		}
		getmouseclick(WM_LBUTTONDOWN, x2, y2);
		while (!ismouseclick(WM_LBUTTONDOWN))
		{
			delay(500);
		}
		getmouseclick(WM_LBUTTONDOWN, x3, y3);

		point *points = new point[numpoints];
		points[0].x = x1;
		points[0].y = y1;
		points[1].x = x2;
		points[1].y = y2;
		points[2].x = x3;
		points[2].y = y3;

		allshapes[counter] = new Triangle (points, color, numpoints, 10);
		allshapes[counter]->draw(1);
		counter++;
		allshapes = increaseArray(allshapes, counter, capacity);

	}
	if (x <= 100 && x >= 0 && y <= 300 && y >= 200)   //is circle?
	{
		numpoints = 1;
		int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
		double r = 0;
		while (!ismouseclick(WM_LBUTTONDOWN))
		{
			delay(500);
		}
		getmouseclick(WM_LBUTTONDOWN, x1, y1);
		while (!ismouseclick(WM_LBUTTONDOWN))
		{
			delay(500);
		}
		getmouseclick(WM_LBUTTONDOWN, x2, y2);

		r = sqrt((x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1));
		
		point *points = new point[1];
		points[0].x = x1;
		points[0].y = y1;

		allshapes[counter] = new Circle (points, color, numpoints, r);
		allshapes[counter]->draw(1);
		counter++;
		allshapes = increaseArray(allshapes, counter, capacity);

	}

	if (x <= 100 && x >= 0 && y <= 400 && y >= 300)  //is solid line?
	{
		int x1, y1, x2, y2;
		numpoints = 2;
		while (!ismouseclick(WM_LBUTTONDOWN))
		{
			delay(500);
		}
		getmouseclick(WM_LBUTTONDOWN, x1, y1);
		while (!ismouseclick(WM_LBUTTONDOWN))
		{
			delay(500);
		}
		getmouseclick(WM_LBUTTONDOWN, x2, y2);

		point *points = new point[numpoints];
		points[0].x = x1;
		points[0].y = y1;
		points[1].x = x2;
		points[1].y = y2;

		allshapes[counter] = new Line (points, color, numpoints, 1);
		allshapes[counter]->draw(1);
		counter++;
		allshapes = increaseArray(allshapes, counter, capacity);
	}

	if (x <= 100 && x >= 0 && y <= 500 && y >= 400)  //is dotted line?
	{
		int x1, y1, x2, y2;
		numpoints = 2;
		while (!ismouseclick(WM_LBUTTONDOWN))
		{
			delay(500);
		}
		getmouseclick(WM_LBUTTONDOWN, x1, y1);
		while (!ismouseclick(WM_LBUTTONDOWN))
		{
			delay(500);
		}
		getmouseclick(WM_LBUTTONDOWN, x2, y2);

		point *points = new point[numpoints];
		points[0].x = x1;
		points[0].y = y1;
		points[1].x = x2;
		points[1].y = y2;

		allshapes[counter] = new Line (points, color, numpoints, 2);
		allshapes[counter]->draw(1);
		counter++;
		allshapes = increaseArray(allshapes, counter, capacity);
	}

	if (x <= 200 && x >= 100 && y <= 300 && y >= 200)  //is user poly?
	{
		numpoints = 0;
		point *pts = new point[100];
		int i = 0, flag = 1;
		while (flag == 1)
		{
			while (!ismouseclick(WM_LBUTTONDOWN))
			{
				delay(500);
			}
			getmouseclick(WM_LBUTTONDOWN, pts[i].x, pts[i].y);
			i++;
			numpoints++;
			if (ismouseclick(WM_RBUTTONDOWN))
			{
				flag = 0;
			}
		}
		pts[i].x = pts[0].x;
		pts[i].y = pts[0].y;
		numpoints++;

		allshapes[counter] = new userpoly (pts, color, numpoints, 10);
		allshapes[counter]->draw(1);
		counter++;
		allshapes = increaseArray(allshapes, counter, capacity);

	}

	if (x <= 200 && x >= 100 && y <= 400 && y >= 300)  //is user curve?
	{
		numpoints = 0;
		point *pts = new point[100];
		int i = 0, flag = 1;
		while (flag == 1)
		{
			while (!ismouseclick(WM_LBUTTONDOWN))
			{
				delay(500);
			}
			getmouseclick(WM_LBUTTONDOWN, pts[i].x, pts[i].y);
			i++;
			numpoints++;
			if (ismouseclick(WM_RBUTTONDOWN))
			{
				flag = 0;
			}
		}

		allshapes[counter] = new curve (pts, color, numpoints, 1);
		allshapes[counter]->draw(1);
		counter++;
		allshapes = increaseArray(allshapes, counter, capacity);

	}

	if (x <= 200 && x >= 100 && y <= 500 && y >= 400)  //is eraser?
	{
		int x1, y1;
		while (!ismouseclick(WM_LBUTTONDOWN))
		{
			delay(500);
		}
		getmouseclick(WM_LBUTTONDOWN, x1, y1);
		point P;
		P.x = x1;
		P.y = y1;
		if (allshapes[counter - 1]->containsPoint(P))
			allshapes[counter - 1]->draw(0);
	}

	if (x <= 200 && x >= 100 && y <= 600 && y >= 500)  //is bucket tool
	{
		int x1, y1;
		while (!ismouseclick(WM_LBUTTONDOWN))
		{
			delay(500);
		}
		getmouseclick(WM_LBUTTONDOWN, x1, y1);
		point P;
		P.x = x1;
		P.y = y1;
		if (allshapes[counter - 1]->containsPoint(P))
			allshapes[counter - 1]->draw(2);
	}
	if (x <= 100 && x >= 0 && y <= 600 && y >= 500) // is text?
	{
		point *p = new point[1];
		int x1, y1;
		char key[2] = { '\0' };
		char text[500] = { '\0' };
		while (!ismouseclick(WM_LBUTTONDOWN))
		{
			delay(500);
		}
		getmouseclick(WM_LBUTTONDOWN, x1, y1);
		moveto(x1, y1);
		int i = 0;
		while (!ismouseclick(WM_LBUTTONDOWN))
		{
			key[0] = getch();
			outtext(key);
			text[i] = key[0];
			i++;
		}
		p[0].x = x1;
		p[0].y = y1;
		allshapes[counter] = new Text (p, color, numpoints, text);
		counter++;
		allshapes = increaseArray(allshapes, counter, capacity);
	}

	if (x <= 300 && x >= 200 && y <= 50 && y >= 0)  //is save?
	{

		for (int i = 0; i < counter - 1; i++)
			allshapes[i]->save();
	}

	if (x <= 300 && x >= 200 && y <= 100 && y >= 50)  //is load?
	{
		;
	}

	if (x <= 300 && x >= 200 && y <= 200 && y >= 100)  //is star?
	{
		point *pts =  new point[2];
		int x1, y1, x2, y2;
		while (!ismouseclick(WM_LBUTTONDOWN))
		{
			delay(500);
		}
		getmouseclick(WM_LBUTTONDOWN, x1, y1);
		while (!ismouseclick(WM_LBUTTONDOWN))
		{
			delay(500);
		}
		getmouseclick(WM_LBUTTONDOWN, x2, y2);
		pts[0].x = x1; pts[0].y = y1; pts[1].x = x2; pts[1].y = y2;

		allshapes[counter] = new star(pts, color, 2, 1);
		allshapes[counter]->draw(1);
		counter++;
		allshapes = increaseArray(allshapes, counter, capacity);
	}

	if (x <= 150 && x >= 100 && y <= 50 && y >= 0)     //yellow
	{
		//outtext("yellow selected");
		color = 14;
	}
	if (x <= 200 && x >= 150 && y <= 50 && y >= 0)     //red
	{
		//outtext("red is selected");
		color = COLOR(178, 34, 34);
	}	
	if (x <= 150 && x >= 100 && y <= 100 && y >= 50)   //black
	{
		//outtext("black is selected");
		color = 0;
	}
	if (x <= 200 && x >= 150 && y <= 100 && y >= 50)   //white
	{
		//outtext("white is selected");
		color = 15;
	}	
	if (x <= 150 && x >= 100 && y <= 150 && y >= 100)  //blue
	{
		//outtext("blue is selected");
		color = COLOR(0, 128, 128);
	}
	if (x <= 200 && x >= 150 && y <= 150 && y >= 100)   //green
	{
		//outtext("green is selected");
		color = COLOR(0, 128, 0);
	}	
	if (x <= 150 && x >= 100 && y <= 200 && y >= 150)    //pink
	{
		//outtext("pink is selected");
		color = COLOR(255, 0, 255);
	}
	if (x <= 200 && x >= 150 && y <= 200 && y >= 150)    //orange
	{
		//outtext("orange selected");
		color = COLOR(255, 69, 0);
	}
	return 1;
}

void main()
{
	initwindow(1200, 600, "Paint Brush");
	panel();
	int x, y, color = 15;
	int mouse, counter = 0;
	shape **allshapes = new shape*[1];
	int capacity = 1;

	while (true)
	{
		while (!ismouseclick(WM_LBUTTONDOWN))
		{
			delay(500);
		}
		getmouseclick(WM_LBUTTONDOWN, x, y);
		mouse = whereismouse(x, y, color, allshapes, counter, capacity);  //returns 0 if mouse is on the screen, 1 if mouse is inside the rectangle, 2 if circle, 3 if triangle, 4 if text, 0 if colour, 
									 //mouse = whereismouseinpanel(x,y); //returns 2 if mouse is in shape, 3 inside colors, 4 inside tools
									 //if (mouse == 2)
									 //	shape = whatshape(x, y); //int whatshape(int x, int y)
									 //if (mouse == 3)
									 //	color = whatcolor(x, y); //int whatcolor(int x, int y)
									 //if (mouse == 4)
									 //	tool = whattool(x, y);  //int whattool(int x, int y)
									 //if (shape > 0 && color > 0 && tool > 0)
									 //{
									 //	draw(shape, color, tool, x, y);  //void draw(int shape, int color, int tool, int x, int y)
									 //}

									 ////if mouse is in the screen do nothing
									 ////if mouse is in the panel check where
		//getch();

		//point a, b;
		//setcolor(WHITE);
		//a.x = 300; a.y = 200; b.x = 600; b.y = 500;
		//mystar(a, b);

	}

}

