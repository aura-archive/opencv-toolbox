/****************************************************
* tlGeometry for ytl's geometry calculate use
* Tianlong Yang, yangtianlong111@gmail.com
* Computer Science & Technology, Tsinghua University
* Copyright (c) 2016.3.29 All rights reserved.
*****************************************************/


//****************
//!!!!
//特别注意,由于某些原因tlPoly中的点是从0开始算的
//!!!!
//****************

#ifndef TLGEOMETRY_H
#define TLGEOMETRY_H

#include <iostream>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <algorithm>
using namespace std;

#ifdef USE_TLGEOMETRY
#define USE_TLPOINT
#define USE_TLLINE
#define USE_TLPOLY
#endif

//double的可以保存的最大数字比long long(int64)要小很多,但是double 保存的是有效数字,并且不会算爆(只是会损失精度),比如1.23*10^100 double是可以装下的 
namespace tl{
	static const int PolygonN = 100005;
	const double EPS=1e-6;
	const double MAXV=1e100;
	const double PI=acos(-1);
	//比较函数 
	int dcmp(const double &a,double b=0){
		if (a-b>EPS) return 1;
		if (b-a>EPS) return -1;
		return 0;
	}
	//四舍五入到k位小数,因为直接用系统自带的输出时会遇到-0.00013保留2位输出-0.00的情况 
	double round(double x,int k=0){
		for (int i=1;i<=k;i++) x*=10;
		x=floor(x+0.5);
		for (int i=1;i<=k;i++) x/=10;
		return x;
	}
	template <class T>
	inline T sqr(T a) {
		return a*a;
	}

	const static int xo[] = { 1,-1,0,0,1,-1,1,-1 };
	const static int yo[] = { 0,0,1,-1,1,-1,-1,1 };

	//点,向量 
	struct tlPoint{
		double x,y;
		tlPoint(double x=0,double y=0):x(x),y(y){

		}
		static tlPoint zero() {
			return tlPoint(0, 0);
		}
		static tlPoint one() {
			return tlPoint(1, 1);
		}
		double len()const {
			return sqrt(sqr(x) + sqr(y));
		}
		double dis(const tlPoint &A)const {
			return sqrt(sqr(A.x - x) + sqr(A.y - y));
		}
		//返回该向量逆时针旋转angle角度后的结果
		tlPoint rotate(const tlPoint &angle) {
			return tlPoint(x*angle.x - y*angle.y, x*angle.y + y*angle.x);
		}//Attention    rotate 90=rotate(tlPoint(0,1));
		void moveTo(int dir) {
			x += xo[dir];
			y += yo[dir];
		}
	};
	tlPoint moveTo(tlPoint p,int dir) {
		return tlPoint(p.x + xo[dir], p.y + yo[dir]);
	}

		
//允许使用别名 tlPoint
#ifdef USE_TLPOINT
	typedef tlPoint Point;
#endif


	double det(const tlPoint &A, const tlPoint &B) {
		return A.x*B.y - A.y*B.x;
	}
	double dot(const tlPoint &A, const tlPoint &B) {
		return A.x*B.x + A.y*B.y;
	}//Atention not '-'!
	tlPoint operator + (const tlPoint &A, const tlPoint &B) {
		return tlPoint(A.x + B.x, A.y + B.y);
	}
	tlPoint operator - (const tlPoint &A, const tlPoint &B) {
		return tlPoint(A.x - B.x, A.y - B.y);
	}
	tlPoint operator * (const double &k, const tlPoint &A) {
		return tlPoint(A.x*k, A.y*k);
	}
	tlPoint operator * (const tlPoint &A, const double &k) {
		return tlPoint(A.x*k, A.y*k);
	}
	tlPoint operator / (const tlPoint &A, const double &k) {
		if (!k) return tlPoint(MAXV, MAXV); return tlPoint(A.x / k, A.y / k);
	}
	bool operator == (const tlPoint &A, const tlPoint &B) {
		return dcmp(A.x, B.x) == 0 && dcmp(A.y, B.y) == 0;
	}
	bool operator != (const tlPoint &A, const tlPoint &B) {
		return dcmp(A.x, B.x) != 0 || dcmp(A.y, B.y) != 0;
	}
	
	bool isZeroPoint(const tlPoint &A) {
		return dcmp(A.x) == 0 && dcmp(A.y) == 0;
	}
	
	double sin(const tlPoint &A, const tlPoint &B) {
		if (dcmp(A.len()*B.len()) == 0) return 0; else return det(A, B) / (A.len()*B.len());
	}//Attention:一锟斤拷要锟叫非凤拷锟斤拷锟斤拷 
	
	double cos(const tlPoint &A, const tlPoint &B) {
		if (A.len()*B.len() == 0) return 0; return dot(A, B) / (A.len()*B.len());
	}
	
	double cross(const tlPoint &A, const tlPoint &B, const tlPoint &C) {
		return det(A, B) + det(B, C) + det(C, A);
	}
	
	//return then cos(A,B) and sin(A,B) in tlPoint type
	tlPoint tlGetAngle(const tlPoint &A, const tlPoint &B) {
		return tlPoint(cos(A, B), sin(A, B));
	}

	//比较向量的角度
	bool cmpAngle(const tlPoint &A, const tlPoint &B) {
		return atan2(A.x, A.y) < atan2(B.x, B.y);
	}
	
	//以x,y为一二关键字比较向量大小
	bool cmpPosWithX(const tlPoint &A, const tlPoint &B) {
		return A.x < B.x || (A.x == B.x&&A.y < B.y);
	}

	tlPoint tlGetMidPoint(const tlPoint &A, const tlPoint &B) {
		return tlPoint((A.x + B.x) / 2, (A.y + B.y) / 2);
	}

	double angle(const tlPoint &A) {
		return atan2(A.y, A.x);
	}//Attention!
	
	double angle(const tlPoint &A, const tlPoint &B) {
		return acos(cos(A, B));
	}
	
	double AngletoRad(const double &a) {
		return a*PI / 180;
	}
	
	double RadtoAngle(const double &a) {
		return a * 180 / PI;
	}
	 
	 

/*****************线******************/
	 
	 
	struct tlLine{
		tlPoint A,dir;
		tlLine(tlPoint A = tlPoint(0, 0), tlPoint dir = tlPoint(0, 0)) :A(A), dir(dir) {
		}
		double disSeg(const tlPoint &P)const;
		int position(const tlPoint &P);
		//线段长度
		double len(const tlPoint &P) {
			tlPoint B(A + dir);
			return fabs(det(P - A, P - B)) / A.dis(B);
		}
	};
#ifdef USE_TLLINE
	typedef tlLine Line;
#endif


	//     ----|------>|---
	//	   1   4   3   5 2    0
	int tlLine::position(const tlPoint &P) {
		if (dir == tlPoint(0, 0)) return 0;
		tlPoint dir2(P - A);
		if (dir2 == tlPoint(0, 0)) return 4;
		if (dcmp(det(dir, dir2)) != 0) return 0;
		if (dcmp(dot(dir, dir2)) < 0) return 1;
		if (dcmp(dir2.len(), dir.len()) < 0) return 3;
		if (dcmp(dir2.len(), dir.len()) > 0) return 2;
		return 5;
	}
    
	//线段与点的距离
	double tlLine::disSeg( const tlPoint &P) const{
		tlPoint B(A + dir);
		if (dcmp(dot(P - A, B - A)) <= 0) return P.dis(A);
		if (dcmp(dot(P - B, A - B)) <= 0) return P.dis(B);
		return fabs(det(P - A, P - B)) / A.dis(B);
	}
	//线段与点的距离
    inline double disSegWithPoint(const tlLine &l,const tlPoint &P){
		return l.disSeg(P);
	}

	//判断直线相交,若相交P为交点
 	bool tlCheckIntersectLine(const tlLine &la,const tlLine &lb,tlPoint &P){
		tlPoint A(la.A),A1(la.A+la.dir),
        	  B(lb.A),B1(lb.A+lb.dir);
		double s1=det(B-A,B1-A),s2=det(B-A1,B1-A1),s=s1-s2;
  		if (dcmp(s)==0) return false;
		P=A+la.dir*s1/s;
		return true;
	}

	//向量P位于向量A,B之间
	bool between(const tlPoint &P, const tlPoint &A, const tlPoint &B) {
		return dcmp(dot(A - P, B - P)) <= 0;
	}

	//判断线段相交,若相交P为交点
	bool tlCheckIntersectSeg(const tlLine &la,const tlLine &lb,tlPoint &P){
		tlPoint A(la.A),A1(la.A+la.dir),
        	  B(lb.A),B1(lb.A+lb.dir);
		int d1=dcmp(det(A1-A,B-A)),d2=dcmp(det(A1-A,B1-A)),d3=dcmp(det(B1-B,A-B)),d4=dcmp(det(B1-B,A1-B));
		if ((d1^d2)==-2&&(d3^d4)==-2){
			double s1=det(B-A,B1-A),s2=det(B-A1,B1-A1);
			P=A+la.dir*s1/(s1-s2);
            return true;
		}
		if (!d1&&between(B, A,A1))  {P=B; return true;}
		if (!d2&&between(B1,A,A1)) {P=B1;return true;}
		if (!d3&&between(A, B,B1))  {P=A; return true;}
		if (!d4&&between(A1,B,B1)) {P=A1;return true;}
		if (!d1&&!d3) return false;
		return false;
	}

	//求垂点
	tlPoint tlGetVertivalPoint(const tlLine &l, const tlPoint &P) {
		tlLine la(P, tlPoint(-l.dir.y, l.dir.x)); 
		tlPoint A;
		tlCheckIntersectLine(l, la, A);
		return A;
	}
	
	
	
/**************多边形****************/	

	//以y为第一关键字,x为第二关键字排序 	
	bool cmpPosWithY(const tlPoint &A, const tlPoint &B) {
		return A.y < B.y || (A.y == B.y&&A.x < B.x);
	}

	struct tlPoly{
		int n;
		tlPoint *a;
		tlPoly() {

		}
		tlPoly(int n) :n(n){
			a = new tlPoint[n + 5];
		}
		tlPoly(int n, tlPoint *aa) :n(n){
			a = new tlPoint[n + 5];
			for (int i = 1; i <= n; i++) {
				a[i] = aa[i];
			}
		}
		~tlPoly() {
			delete a;
		}
		//绕p点旋转angle角度
		void rotate(const tlPoint &angle, tlPoint P = tlPoint::zero()) {
			for (int i = 1; i <= n; i++) a[i] = P + (a[i] - P).rotate(angle);
		}
		tlPoly narrow(const double &x);
		//求面积 
		double getArea(){
			double area=0;
			a[n+1]=a[1];
			for (int i=1;i<=n;i++) area+=(a[i].x-a[i+1].x)*(a[i].y+a[i+1].y);
			return fabs(area)*0.5;
		}

		//求周长 
		double getCircum(){
			double cir=0;
			a[n+1]=a[1];
			for (int i=1;i<=n;i++) cir+=a[i].dis(a[i+1]);
			return cir;
		}

		//求重心 
		tlPoint getGravity(){
			tlPoint G;double S=0,tot=0;
			a[n+1]=a[1];
			for (int i=1;i<=n;i++){
				S=det(a[i],a[i+1]);
				G=G+S*(a[i]+a[i+1])/3;
				tot+=S;
			}
			G=G/tot;
			return G;
		}
		
		//求凸包 
		void getConvex() {
			sort(a + 1, a + n + 1, cmpPosWithY);
			static tlPoint d[PolygonN];
			int p1 = 0;
			for (int i = 1; i <= n; i++) {
				while (p1>1 && dcmp(det(d[p1] - d[p1 - 1], a[i] - d[p1])) <= 0) p1--;
				d[++p1] = a[i];
			}
			int p2 = p1;
			for (int i = n; i >= 1; i--) {
				while (p2>p1&&dcmp(det(d[p2] - d[p2 - 1], a[i] - d[p2])) <= 0) p2--;
				d[++p2] = a[i];
			}
			n = p2 - 1;
			for (int i = 1; i <= n; i++) a[i] = d[i];
		}
	
		//凸包多边形求直径
		double getConvexDiameter() {
			if (n<3) return a[1].dis(a[n]);
			double ans = 0;
			a[n + 1] = a[1];
			int p = 2;//Attention:p=2!
			for (int i = 1; i <= n; i++) {
				while (dcmp(cross(a[i], a[i + 1], a[p]), cross(a[i], a[i + 1], a[p + 1]))<0) p = p%n + 1;//Attention!!!
				ans = max(ans, a[i].dis(a[p]));
			}
			return ans;
		}

		//凸包多边形求宽
		double getConvexWide() {
			if (n<3) return 0;//Attention!!!
			int t = 2; double ans = MAXV; a[n + 1] = a[1];
			for (int i = 1; i <= n; i++) {
				while (dcmp(cross(a[i], a[i + 1], a[t]), cross(a[i], a[i + 1], a[t + 1]))<0) t = t%n + 1;//Attention!!!
				ans = min(ans,tlLine(a[i], a[i + 1] - a[i]).disSeg(a[t]));
			}
			return ans;
		}


		//判断点是否在多边形内 
		bool checkPointIn(const tlPoint &P){            
   	    	a[n+1]=a[1];
			for (int i=1;i<=n;i++) if (dcmp(dot(a[i]-P,a[i+1]-P))<=0&&dcmp(det(a[i]-P,a[i+1]-P))==0) return true;
			int tot=0;           
   	    	for (int i=1;i<=n;i++){
				int k1=dcmp(a[i].x-P.x);
					int k2=dcmp(a[i+1].x-P.x);
					int k3=dcmp(det(a[i+1]-a[i],P-a[i]));
					if (k1<=0&&k2>0&&k3>0) tot++;
					if (k2<=0&&k1>0&&k3<0) tot++;
			}
			return tot&1;
		}
		
		//最小面积外接矩形
		double getMinimumAreaBoundingRectangle() {
			if (n<3) return 0;//Attention!!!
			int t1 = 2, t2 = 2, t3 = 1;
			tlPoint P1, P2, P3; tlLine l;
			double area = MAXV, x, y;
			a[n + 1] = a[1];
			while (dcmp(dot(a[t3 + 1] - a[1], a[2] - a[1]), dot(a[t3] - a[1], a[2] - a[1])) >= 0) t3 = t3%n + 1;
			for (int i = 1; i <= n; i++) {
				while (dcmp(cross(a[i], a[i + 1], a[t2]), cross(a[i], a[i + 1], a[t2 + 1]))<0) t2 = t2%n + 1;//Attention!!!
				while (dcmp(dot(a[t1 + 1] - a[i], a[i + 1] - a[i]), dot(a[t1] - a[i], a[i + 1] - a[i]))>0) t1 = t1%n + 1;
				while (dcmp(dot(a[t3 + 1] - a[i], a[i + 1] - a[i]), dot(a[t3] - a[i], a[i + 1] - a[i]))<0) t3 = t3%n + 1;
				l = tlLine(a[i], a[i + 1] - a[i]);
				P1 = tlGetVertivalPoint(l, a[t1]);
				P2 = tlGetVertivalPoint(l, a[t2]);
				P3 = tlGetVertivalPoint(l, a[t3]);
				x = P2.dis(a[t2]);
				y = P1.dis(P3);
				area = min(area, x*y);
			}
			return area;
		}

		//最小周长外接矩形
#define MCBR MinimumCircumBoundingRectangle
		double tlPoly::getMinimumCircumBoundingRectangle() {
			if (n<3) return 0;
			double ans = MAXV;
			a[n + 1] = a[1];
			int p1 = 2, p2 = 2, p3 = 2;
			while (dcmp(dot(a[2] - a[1], a[p3] - a[1]), dot(a[2] - a[1], a[p3 + 1] - a[1])) <= 0) p3 = p3%n + 1;
			for (int i = 1; i <= n; i++) {
				while (dcmp(dot(a[i + 1] - a[i], a[p1] - a[i]), dot(a[i + 1] - a[i], a[p1 + 1] - a[i]))<0) p1 = p1%n + 1;
				while (dcmp(det(a[i + 1] - a[i], a[p2] - a[i]), det(a[i + 1] - a[i], a[p2 + 1] - a[i]))<0) p2 = p2%n + 1;
				while (dcmp(dot(a[i + 1] - a[i], a[p3] - a[i]), dot(a[i + 1] - a[i], a[p3 + 1] - a[i]))>0) p3 = p3%n + 1;
				tlLine l(a[i], a[i + 1] - a[i]);
				tlPoint A = tlGetVertivalPoint(l, a[p1]), B = tlGetVertivalPoint(l, a[p2]), C = tlGetVertivalPoint(l, a[p3]);
				double x = A.dis(C), y = B.dis(a[p2]);
				ans = min(ans, x + x + y + y);
			}
			return ans;
		}

		//最大内接三角形 
#define MIT MaximumInscribedTriangle 
		double getMaximumInscribedTriangle() {
			a[n + 1] = a[1]; double area = 0;
			int p = 2;
			for (int i = 1; i <= n + 1; i++) {
				while (p <= n&&dcmp(det(a[i + 1] - a[i], a[p] - a[i]), det(a[i + 1] - a[i], a[p + 1] - a[i]))<0) p++;
				area = max(area, det(a[i + 1] - a[i], a[p] - a[i]));
				int k = p;
				for (int j = i + 2; j <= n + 1; j++) {
					while (k <= n&&dcmp(det(a[j] - a[i], a[k] - a[i]), det(a[j] - a[i], a[k + 1] - a[i]))<0) k++;
					area = max(area, det(a[j] - a[i], a[k] - a[i]));
				}
			}
			return area*0.5;
		}

		//点到多边形距离(点在多边形外)
		double tlPoly::dis(const tlPoint &P) {
			double ans = 0x7fffffff; a[n + 1] = a[1];
			for (int i = 1; i <= n; i++) ans = min(ans, tlLine(a[i], a[i + 1] - a[i]).disSeg(P));//Attention!
			return ans;
		}

};
	
#ifdef USE_TLPOLY
typedef tlPoly Poly;
#endif


	
/**************旋转卡壳****************/	


	double _ConvexDisMin(tlPoly &a,tlPoly &b){
		a.a[a.n+1]=a.a[1];b.a[b.n+1]=b.a[1];
		int t=1;double ans=MAXV;
		for (int i=1;i<=a.n;i++){
			tlLine l(a.a[i],a.a[i+1]-a.a[i]);
			while (dcmp(det(l.dir,b.a[t]-a.a[i]),det(l.dir,b.a[t+1]-a.a[i]))<0) t=t%b.n+1;	
			ans=min(ans,min(l.disSeg(b.a[t]),l.disSeg(b.a[t+1])));//Attention!	
		}
		
		return ans;
	}

	double _ConvexDisMax(tlPoly &a,tlPoly &b){
		a.a[a.n+1]=a.a[1];b.a[b.n+1]=b.a[1];
		int t=1;double ans=0;
		for (int i=1;i<=a.n;i++){
			tlLine l(a.a[i],a.a[i+1]-a.a[i]);
			while (dcmp(det(l.dir,b.a[t]-a.a[i]),det(l.dir,b.a[t+1]-a.a[i]))<0) t=t%b.n+1;	
			ans=max(ans,a.a[i].dis(b.a[t]));ans=max(ans,a.a[i].dis(b.a[t+1]));	
		}
		return ans;
	}

	//凸包间最小距离
	double tlGetConvexDisMin(tlPoly &a, tlPoly &b) {
		return min(_ConvexDisMin(a, b), _ConvexDisMin(b, a));
	}
	//凸包间最大距离
	double tlGetConvexDisMax(tlPoly &a, tlPoly &b) {
		return min(_ConvexDisMax(a, b), _ConvexDisMax(b, a));
	}

/**************半平面交*******************/	
	
	//直线切多边形
	void _cut(tlPoly &a,const tlLine &l){
		//EPS=1e-4;
		static tlPoly b;b.n=0;tlPoint P;a.a[a.n+1]=a.a[1];//Attention!
		for (int i=1;i<=a.n;i++){
			if (dcmp(det(l.dir,a.a[i]-l.A))>=0) b.a[++b.n]=a.a[i];
			tlLine la(a.a[i],a.a[i+1]-a.a[i]);
			if (tlCheckIntersectLine(la,l,P)&&between(P,a.a[i],a.a[i+1])) b.a[++b.n]=P;//Important Attention:intersect()要锟斤拷凸锟斤拷锟较碉拷锟斤拷锟斤拷锟叫革拷锟竭ｏ拷锟斤拷锟斤拷锟叫撅拷锟斤拷锟斤拷锟斤拷! 
		} 
		a.n=b.n;for (int i=1;i<=b.n;i++) a.a[i]=b.a[i];
	}

	//半平面交NlogN
	//dcmp()>0 当半平面交交成一个点的时候无法算出-->判定是否有交会出问题 
	//dcmp()>=0 会有重复的顶点-->算面积不会有问题 
	bool _cmpLine(const tlLine &la, const tlLine &lb) {
		return dcmp(atan2(la.dir.y, la.dir.x), atan2(lb.dir.y, lb.dir.x)) < 0;
	}
	bool _onLeft(const tlPoint &P, const tlLine &l) {
		return dcmp(det(l.dir, P - l.A)) >= 0;
	}
	void HalfplaneIntersect(tlLine l[],int n,tlPoly &a){
		static tlPoint P[PolygonN];
		static tlLine Q[PolygonN];
		sort(l+1,l+n+1,_cmpLine);//Attention!
		Q[1]=l[1];int p1=1,p2=1;
		for (int i=2;i<=n;i++){
			while (p1<p2&&!_onLeft(P[p2-1],l[i])) p2--;
			while (p1<p2&&!_onLeft(P[p1],l[i])) p1++;
			Q[++p2]=l[i];
			if (dcmp(det(Q[p2].dir,Q[p2-1].dir))==0){
				if (_onLeft(l[i].A,Q[p2-1])) Q[p2-1]=l[i];
				p2--;
			}	
			if (p1<p2) tlCheckIntersectLine(Q[p2],Q[p2-1],P[p2-1]);
		}
		while (p1<p2&&!_onLeft(P[p2-1],Q[p1])) p2--;
		if (p1<p2) tlCheckIntersectLine(Q[p2],Q[p1],P[p2]);
		a.n=0;
		if (p2-p1<=1) return;
		for (int i=p1;i<=p2;i++) a.a[++a.n]=P[i];
	}	

	//多边形求核NlogN 
	void getPolyCore(tlPoly &a){
		static tlLine l[PolygonN];a.a[a.n+1]=a.a[1];
		for (int i=1;i<=a.n;i++) l[i]=tlLine(a.a[i],a.a[i+1]-a.a[i]);
		HalfplaneIntersect(l,a.n,a);
	}
	//多边形内缩NlogN 
	void getPolyNarrow(tlPoly &a,double x,tlPoly &b){
		static tlLine l[PolygonN];
		a.a[a.n+1]=a.a[1];
		for (int i=1;i<=a.n;i++){
			l[i]=tlLine(a.a[i],a.a[i+1]-a.a[i]);
			l[i].A=l[i].A+l[i].dir.rotate(tlPoint(0,1))*x/l[i].dir.len();
		}
		HalfplaneIntersect(l,a.n,b);
	}
/**********************************/		
	
	//三角形外心 
	tlPoint circumCenter(const tlPoint &A,const tlPoint &B,const tlPoint &C){
		tlPoint D=tlGetMidPoint(A,B),E=tlGetMidPoint(B,C),P;
		tlLine la(D,(B-A).rotate(tlPoint(0,1)));
		tlLine lb(E,(C-B).rotate(tlPoint(0,1)));
		tlCheckIntersectLine(la,lb,P);
		return P;
	}
	//三角形内心 
	tlPoint innerCenter(const tlPoint &A,const tlPoint &B,const tlPoint &C){
		double a=B.dis(C),b=C.dis(A),c=A.dis(B);
		tlPoint P=(A*a+B*b+C*c)/(a+b+c);
		return P;
	}
	

/*****************圆******************/	
	//圆 
	struct Circle{
		tlPoint O;double r;
		Circle(tlPoint O = tlPoint(0, 0), double r = 0) :O(O), r(r) {
		};

		//圆上位于弧度a处的点的位置
		tlPoint pointOnCircleWithAngle(double a)const {
			return tlPoint(O.x + r*std::cos(a), O.y + r*std::sin(a));
		}

		bool checkCircleIn(const tlPoint &A) {
			return dcmp(A.dis(O), r) < 0;
		}//Attention::锟较革拷锟斤拷锟斤拷 
		
		double Circle::sectorArea(const tlPoint &A, const tlPoint &B)const {
			return angle(A - O, B - O)*sqr(r) / 2;
		}

		//暂不维护
		//double Circle::intersectArea(const tlPoint &A1, const tlPoint &B1) {
		//	tlPoint A(A1), B(B1); Circle c(O, r);
		//	int t = dcmp(cross(O, A, B)), t1 = circleIn(A), t2 = circleIn(B);
		//	if (!t) return 0;
		//	if (t1&&t2) return cross(O, A, B)*0.5;
		//	if ((!t1) && (!t2)) {
		//		double ans = sectorArea(A, B);
		//		tlLine l(A, B - A);//tlPoint P=projection(l,O);(锟斤拷锟斤拷锟斤拷锟筋，锟斤拷锟斤拷锟斤拷)
		//		if (dcmp(l.disSeg(O), r)<0) {
		//			tlPoint v[3]; int tot = 0;
		//			intersectLine(c, l, v, tot);
		//			ans = ans - sectorArea(v[1], v[2]) + fabs(cross(O, v[1], v[2]))*0.5;
		//		}
		//		return ans*t;
		//	}
		//	if (t2) swap(A, B);
		//	tlPoint C, D;
		//	intersectRay(c, tlLine(A, B - A), C);
		//	intersectRay(c, tlLine(O, B - O), D);
		//	double ans = fabs(cross(O, A, C))*0.5 + sectorArea(C, D);
		//	return ans*t;
		//}
		////圆锟斤拷锟斤拷锟斤拷锟轿斤拷锟斤拷锟斤拷锟斤拷 
		//double Circle::intersectArea(tlPoly &a) {
		//	a.a[a.n + 1] = a.a[1]; double ans = 0;
		//	for (int i = 1; i <= a.n; i++) ans += intersectArea(a.a[i], a.a[i + 1]);
		//	return fabs(ans);
		//}
	};

	//圆相交
	void circleIntersect(const Circle &c1,const Circle &c2,tlPoint v[],int &tot){
		if (isZeroPoint(c1.O-c2.O)&&dcmp(c1.r,c2.r)==0) {tot=-1;return;}
		double d=c1.O.dis(c2.O);
		if (dcmp(c1.r+c2.r,d)<0) return;
		if (dcmp(fabs(c1.r-c2.r),d)>0) return;
		double a=angle(c2.O-c1.O);
		double da=acos((sqr(c1.r)+sqr(d)-sqr(c2.r))/(2*c1.r*d));
		v[++tot]=c1.pointOnCircleWithAngle(a+da);v[++tot]=c1.pointOnCircleWithAngle(a-da);
		if (isZeroPoint(v[tot]-v[tot-1])) tot--;
	}
	//暂不维护
	//void intersectRay(const Circle &c,const tlLine &l,tlPoint &A){
	//	tlPoint P=projection(l,c.O);
	//	double d=P.dis(c.O),x=sqrt(sqr(c.r)-sqr(d));
	//	A=P+l.dir*x/l.dir.len();
	//}	


	//暂不维护 
	//void intersectLine(const Circle &c,const tlLine &l,tlPoint v[],int &tot){//Attenrion::&tot
	//	tlPoint P=projection(l,c.O);
	//	if (tlPointZero(P-c.O)){
	//		
	//		tlPoint dir=l.dir*c.r/l.dir.len();
	//		v[++tot]=P+dir;
	//		v[++tot]=P-dir;
	//		return;
	//	}
	//	double d=P.dis(c.O);
	//	if (dcmp(d,c.r)>0) return;
	//	if (dcmp(d,c.r)==0) {v[++tot]=P;return;}
	//	double a=angle(P-c.O),da=acos(d/c.r);
	//	v[++tot]=c.tlPoint(a+da);
	//	v[++tot]=c.tlPoint(a-da);
	//}

	//暂不维护
	////圆锟斤拷圆锟斤拷锟斤拷锟斤拷锟斤拷 
	//double intersectArea(Circle &c1,Circle &c2){
	//	if (c1.r<c2.r) swap(c1,c2);
	//	double d=c1.O.dis(c2.O);
	//	if (dcmp(c1.r+c2.r,d)<0) return 0;
	//	if (dcmp(c1.r-c2.r,d)>=0) return PI*sqr(c2.r);
	//	//double x1=c1.O.x,y1=c1.O.y,r1=c1.r,
	//	//       x2=c2.O.x,y2=c2.O.y,r2=c2.r;//???
	//	double a1=2*acos((sqr(c1.r)+sqr(d)-sqr(c2.r))/(2*c1.r*d)),a2=2*acos((sqr(c2.r)+sqr(d)-sqr(c1.r))/(2*c2.r*d));
	//	return a1*sqr(c1.r)*0.5+a2*sqr(c2.r)*0.5-std::sin(a1)*sqr(c1.r)*0.5-std::sin(a2)*sqr(c2.r)*0.5;
	//}
	////锟斤拷锟斤拷锟斤拷锟斤拷圆锟斤拷锟斤拷锟斤拷(锟斤拷锟斤拷锟斤拷锟竭凤拷锟斤拷锟斤拷锟? 
	//void Tangent(const Circle &c,const tlPoint &P,tlPoint v[],int &tot){
	//	tlPoint A=c.O-P;
	//	double d=A.len();
	//	if (dcmp(c.r,d)>0) return;
	//	if (dcmp(c.r,d)==0){v[++tot]=A.rotate(PI/2);return;}
	//	double a=asin(c.r/d);
	//	v[++tot]=A.rotate(a);
	//	v[++tot]=A.rotate(-a);
	//}
	////???
	////锟皆诧拷锟斤拷锟?锟斤拷锟斤拷锟叫碉拷) 
	//void Tangent(Circle &c1,Circle &c2,tlPoint v1[],tlPoint v2[],int &tot){
	//	if (tlPointZero(c1.O-c2.O)&&dcmp(c1.r,c2.r)==0) {tot=-1;return;}
	//	int t=0;
	//	if (c1.r<c2.r){swap(c1,c2);swap(v1,v2);t=1;}
	//	double d=c1.O.dis(c2.O);
	//	if (dcmp(c1.r-c2.r,d)>0) return;
	//	double a=angle(c2.O-c1.O);
	//	if (dcmp(c1.r-c2.r,d)==0){
	//		v1[++tot]=c1.tlPoint(a);v2[tot]=c2.tlPoint(a);return;
	//	}
	//	double da=acos((c1.r-c2.r)/d);
	//	v1[++tot]=c1.tlPoint(a+da);v2[tot]=c2.tlPoint(a+da);
	//	v1[++tot]=c1.tlPoint(a-da);v2[tot]=c2.tlPoint(a-da);
	//	if (dcmp(c1.r+c2.r,d)==0){
	//		v1[++tot]=c1.tlPoint(a);v2[tot]=c2.tlPoint(PI+a);
	//	}
	//	if (dcmp(c1.r+c2.r,d)<0){
	//		double da=acos((c1.r+c2.r)/d);
	//		v1[++tot]=c1.tlPoint(a+da);v2[tot]=c2.tlPoint(PI+a+da);
	//		v1[++tot]=c1.tlPoint(a-da);v2[tot]=c2.tlPoint(PI+a-da);
	//	}
	//	if (t){swap(c1,c2);swap(v1,v2);}
	//	return;
	//}
	////锟斤拷小圆锟斤拷锟斤拷[锟斤拷锟斤拷锟斤拷锟絔 
	//void MinimumCircle(tlPoint a[],int n,Circle &c){
	//	random_shuffle(a+1,a+n+1);
	//	tlPoint O=a[1];double r=0;
	//	for (int i=2;i<=n;i++){
	//		if (dcmp(O.dis(a[i]),r)<=0) continue;
	//		O=a[i];r=0;
	//		for (int j=1;j<i;j++){
	//			if (dcmp(O.dis(a[j]),r)<=0) continue;
	//			O=midtlPoint(a[i],a[j]);
	//			r=O.dis(a[i]);
	//			for (int k=1;k<j;k++){
	//				if (dcmp(O.dis(a[k]),r)<=0) continue;
	//				O=circumCenter(a[i],a[j],a[k]);
	//				r=O.dis(a[i]);
	//			}
	//		}
	//	}
	//	c=Circle(O,r);
	//}
}

/*int main(){
	freopen("Geometry.in","r",stdin);
	freopen("Geometry.out","w",stdout);
	
	tlLine l1(tlPoint(0,0),tlPoint(2,2));
	tlLine l2(tlPoint(1,1),tlPoint(2,2));
	tlPoint P;
	printf("%d\n",intersectSeg(l1,l2,P));
	P.print();
	return 0;
}*/



#endif