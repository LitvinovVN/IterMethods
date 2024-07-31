// azov3d.cpp : Defines the entry point for the console application.
// g++ PTM_New.cpp -o PTM_New
// ./PTM_New

#include <iostream>
#include <math.h>
#include <stdio.h>

using namespace std;

int Nx = 355;
int Ny = 233;
int Nz = 36;

int n3d = Nx * Ny * Nz;
int n2d = Nx * Ny;

void vvod(double*, double*, double*,
	double*, double*, double*,
	double*, double*, double*,
	double*, int*);

void vvodmask(double*, double*, double*,
	double*, double*, double*, int*);

void vivod(double*, double*, double*, double*);

void firstfunc(double*, double*,
	double*, double*, double*, double*, int*);

void ptmss(double*, double*, double*, double*, double*,
	double*, double*, double*,
	double*, int*, double, double);

void sweep_right(double* r, double * d0, double * d5, double * d6, double* f, double* c2, double* c4, double omega);
void sweep_left(double* r, double * d0, double * d5, double * d6, double* f, double* c0, double* c1, double* c3, double omega);

double      dt = 500;
double      t = 0;
double      dx = 1000;//6800/n1;
double      dy = 1000;//280/n2;
double      dz = 0.5;//280/n2;
double      t0 = 87000;
double      pa = 1.29;
double      po = 1000;
double      pv = 2700;
double      ev = 0.00000000001;
double      ep = 0.00000000001;
double      ek = 0.000001;
double      kor = 0.000103;
double      cp_v = 0.0088;
double      cp_a = 0.0026;
double      g = 9.81;

//ветер на 16.06.2021 06:00, северный, 3 м/с
double		sk = 3;
double		alfa = 45;
double      sk_u = (-1) * sk;
double      sk_v = 0;

double      ca = pa * cp_a / po;
double      cv = pv * cp_v / po;
double      isp = 606 / (38869 * dx * dy);


int main(int argc, char* argv[])
{

	double* u = new double[n3d];
	double* v = new double[n3d];
	double* w = new double[n3d];
	double* a = new double[n3d];
	double* a1 = new double[n2d];
	double* mu = new double[n3d];
	double* nu = new double[n3d];
	double* o_max = new double[n3d];
	double* o = new double[n3d];
	double* h = new double[n2d];

	int* s1 = new int[n3d];

	int k1;

	printf("mass OK\n");

	vvod(u, v, w, a, a1, mu, nu, o, o_max, h, s1);
	printf("vvod OK\n");

	do
	{
		vvodmask(u, v, w, a, o, o_max, s1);
		firstfunc(u, v, w, mu, nu, o, s1);

		t = t + dt;

		vivod(u, v, w, a);
		printf("vivod OK\n");

	} while (t < t0);


	delete[] u;
	delete[] v;
	delete[] w;
	delete[] a;
	delete[] a1;
	delete[] s1;
	delete[] mu;
	delete[] nu;
	delete[] o_max;
	delete[] o;
	delete[] h;

	printf("del OK\n");

	return 0;
}

void vvodmask(double* u, double* v, double* w,
	double* a, double* o, double* o_max, int* s)

{
	int i, j, k;
	int m0, m2, m4, m5, m6,
		m24, m26, m46, m246, m15, m35, m135;

	double k0, k1, k2, k3, k4;
	double sum = 0;


	for (k = 1;k < Nz - 2;k++)
		for (j = 1;j < Ny - 2;j++)
			for (i = 1;i < Nx - 2;i++)
			{
				m0 = k + Nz * j + Ny * Nz * i;
				m5 = m0 + 1;
				m15 = m0 + Ny * Nz + 1;
				m35 = m0 + Nz + 1;
				m135 = m0 + Ny * Nz + Nz + 1;


				o[m0] = (a[m5] + a[m15] + a[m35] + a[m135]) / (4 * po * g * dz) + k - 5;

				if (o[m0] > o_max[m0])
					o[m0] = o_max[m0];

				if (o[m0] < 0)
					o[m0] = 0;

			}

	for (k = 1;k < Nz - 1;k++)
		for (i = 1;i < Nx - 1;i++)
			for (j = 1;j < Ny - 1;j++)
			{
				m0 = k + Nz * j + Ny * Nz * i;
				m2 = m0 - Ny * Nz;
				m4 = m0 - Nz;
				m6 = m0 - 1;
				m24 = m0 - Ny * Nz - Nz;
				m26 = m0 - Ny * Nz - 1;
				m46 = m0 - Nz - 1;
				m246 = m0 - Ny * Nz - Nz - 1;

				k1 = 0.25 * (o[m0] + o[m4] + o[m6] + o[m46]);
				k2 = 0.25 * (o[m2] + o[m24] + o[m26] + o[m246]);
				k0 = 0.5 * (k1 + k2);

				if ((k0 > 0) && (s[m0] == 0))
					a[m0] = po * g * dz * (5 - k);

				s[m0] = 0;
				if (k0 > 0)
					s[m0] = 1;
				if (k0 == 0)
				{
					a[m0] = 0;
					u[m0] = 0;
					v[m0] = 0;
					w[m0] = 0;
				}

			}

	j = 132;    // cиваш
	i = 1;
	for (k = 1;k < Nz;k++)
	{
		m0 = k + Nz * j + Ny * Nz * i;
		m2 = m0 - Ny * Nz;
		m4 = m0 - Nz;
		m6 = m0 - 1;
		m24 = m0 - Ny * Nz - Nz;
		m26 = m0 - Ny * Nz - 1;
		m46 = m0 - Nz - 1;
		m246 = m0 - Ny * Nz - Nz - 1;

		k1 = 0.25 * (o[m0] + o[m4] + o[m6] + o[m46]);
		k2 = 0.25 * (o[m2] + o[m24] + o[m26] + o[m246]);

		sum = sum + k1 - k2;
	}


	for (k = 1;k < Nz;k++)
	{
		m0 = k + Nz * j + Ny * Nz * i;

		if (s[m0] > 0)
		{
			u[m0] = -115 / (dy * dz * sum);
			v[m0] = 0;
			s[m0] = 2;
		}
	}


	sum = 0;
	j = 6;    // гирла мертвый донец и средняя кутерьма
	i = 348;
	for (k = 1;k < Nz;k++)
	{
		m0 = k + Nz * j + Ny * Nz * i;
		m2 = m0 - Ny * Nz;
		m4 = m0 - Nz;
		m6 = m0 - 1;
		m24 = m0 - Ny * Nz - Nz;
		m26 = m0 - Ny * Nz - 1;
		m46 = m0 - Nz - 1;
		m246 = m0 - Ny * Nz - Nz - 1;

		k1 = 0.25 * (o[m0] + o[m4] + o[m6] + o[m46]);
		k2 = 0.25 * (o[m2] + o[m24] + o[m26] + o[m246]);

		sum = sum + k2 - k1;
	}


	for (k = 1;k < Nz;k++)
	{
		m0 = k + Nz * j + Ny * Nz * i;

		if (s[m0] > 0)
		{
			u[m0] = -390 / (dy * dz * sum);
			v[m0] = 0;
			s[m0] = 2;
		}
	}


	sum = 0;
	j = 10;    // гирло кутерьма
	i = 347;
	for (k = 1;k < Nz;k++)
	{
		m0 = k + Nz * j + Ny * Nz * i;
		m2 = m0 - Ny * Nz;
		m4 = m0 - Nz;
		m6 = m0 - 1;
		m24 = m0 - Ny * Nz - Nz;
		m26 = m0 - Ny * Nz - 1;
		m46 = m0 - Nz - 1;
		m246 = m0 - Ny * Nz - Nz - 1;

		k1 = 0.25 * (o[m0] + o[m4] + o[m6] + o[m46]);
		k2 = 0.25 * (o[m2] + o[m24] + o[m26] + o[m246]);

		sum = sum + k2 - k1;
	}


	for (k = 1;k < Nz;k++)
	{
		m0 = k + Nz * j + Ny * Nz * i;

		if (s[m0] > 0)
		{
			u[m0] = -424 / (dy * dz * sum);
			v[m0] = 0;
			s[m0] = 2;
		}
	}


	sum = 0;
	j = 16;    // гирло мокрая кутерьма
	i = 348;
	for (k = 1;k < Nz;k++)
	{
		m0 = k + Nz * j + Ny * Nz * i;
		m2 = m0 - Ny * Nz;
		m4 = m0 - Nz;
		m6 = m0 - 1;
		m24 = m0 - Ny * Nz - Nz;
		m26 = m0 - Ny * Nz - 1;
		m46 = m0 - Nz - 1;
		m246 = m0 - Ny * Nz - Nz - 1;

		k1 = 0.25 * (o[m0] + o[m4] + o[m6] + o[m46]);
		k2 = 0.25 * (o[m2] + o[m24] + o[m26] + o[m246]);

		sum = sum + k2 - k1;
	}


	for (k = 1;k < Nz;k++)
	{
		m0 = k + Nz * j + Ny * Nz * i;

		if (s[m0] > 0)
		{
			u[m0] = -185 / (dy * dz * sum);
			v[m0] = 0;
			s[m0] = 2;
		}
	}


	sum = 0;
	j = 19;    // гирло мериновое
	i = 348;
	for (k = 1;k < Nz;k++)
	{
		m0 = k + Nz * j + Ny * Nz * i;
		m2 = m0 - Ny * Nz;
		m4 = m0 - Nz;
		m6 = m0 - 1;
		m24 = m0 - Ny * Nz - Nz;
		m26 = m0 - Ny * Nz - 1;
		m46 = m0 - Nz - 1;
		m246 = m0 - Ny * Nz - Nz - 1;

		k1 = 0.25 * (o[m0] + o[m4] + o[m6] + o[m46]);
		k2 = 0.25 * (o[m2] + o[m24] + o[m26] + o[m246]);

		sum = sum + k2 - k1;
	}


	for (k = 1;k < Nz;k++)
	{
		m0 = k + Nz * j + Ny * Nz * i;

		if (s[m0] > 0)
		{
			u[m0] = -105 / (dy * dz * sum);
			v[m0] = 0;
			s[m0] = 2;
		}
	}


	sum = 0;
	j = 22;    // гирло песчаное
	i = 350;
	for (k = 1;k < Nz;k++)
	{
		m0 = k + Nz * j + Ny * Nz * i;
		m2 = m0 - Ny * Nz;
		m4 = m0 - Nz;
		m6 = m0 - 1;
		m24 = m0 - Ny * Nz - Nz;
		m26 = m0 - Ny * Nz - 1;
		m46 = m0 - Nz - 1;
		m246 = m0 - Ny * Nz - Nz - 1;

		k1 = 0.25 * (o[m0] + o[m4] + o[m6] + o[m46]);
		k2 = 0.25 * (o[m2] + o[m24] + o[m26] + o[m246]);

		sum = sum + k2 - k1;
	}


	for (k = 1;k < Nz;k++)
	{
		m0 = k + Nz * j + Ny * Nz * i;

		if (s[m0] > 0)
		{
			u[m0] = -199 / (dy * dz * sum);
			v[m0] = 0;
			s[m0] = 2;
		}
	}


	sum = 0;
	j = 29;    // гирла свиное, кривое и богдан
	i = 353;
	for (k = 1;k < Nz;k++)
	{
		m0 = k + Nz * j + Ny * Nz * i;
		m2 = m0 - Ny * Nz;
		m4 = m0 - Nz;
		m6 = m0 - 1;
		m24 = m0 - Ny * Nz - Nz;
		m26 = m0 - Ny * Nz - 1;
		m46 = m0 - Nz - 1;
		m246 = m0 - Ny * Nz - Nz - 1;

		k1 = 0.25 * (o[m0] + o[m4] + o[m6] + o[m46]);
		k2 = 0.25 * (o[m2] + o[m24] + o[m26] + o[m246]);

		sum = sum + k2 - k1;
	}


	for (k = 1;k < Nz;k++)
	{
		m0 = k + Nz * j + Ny * Nz * i;

		if (s[m0] > 0)
		{
			u[m0] = -82 / (dy * dz * sum);
			v[m0] = 0;
			s[m0] = 2;
		}
	}


	sum = 0;
	j = 223;    // кубань
	i = 207;
	for (k = 1;k < Nz;k++)
	{
		m0 = k + Nz * j + Ny * Nz * i;
		m2 = m0 - Ny * Nz;
		m4 = m0 - Nz;
		m6 = m0 - 1;
		m24 = m0 - Ny * Nz - Nz;
		m26 = m0 - Ny * Nz - 1;
		m46 = m0 - Nz - 1;
		m246 = m0 - Ny * Nz - Nz - 1;

		k3 = 0.25 * (o[m0] + o[m2] + o[m6] + o[m26]);
		k4 = 0.25 * (o[m4] + o[m24] + o[m46] + o[m246]);

		sum = sum + k4 - k3;
	}


	for (k = 1;k < Nz;k++)
	{
		m0 = k + Nz * j + Ny * Nz * i;

		if (s[m0] > 0)
		{
			u[m0] = 0;
			v[m0] = -923 / (dx * dz * sum);
			s[m0] = 2;
		}
	}

	j = 231;
	for (i = 1;i < Nx;i++)
		for (k = 1;k < Nz;k++)
		{
			m0 = k + Nz * j + Ny * Nz * i;
			m2 = m0 - Ny * Nz;
			m4 = m0 - Nz;
			m6 = m0 - 1;
			m24 = m0 - Ny * Nz - Nz;
			m26 = m0 - Ny * Nz - 1;
			m46 = m0 - Nz - 1;
			m246 = m0 - Ny * Nz - Nz - 1;

			k3 = 0.25 * (o[m0] + o[m2] + o[m6] + o[m26]);
			k4 = 0.25 * (o[m4] + o[m24] + o[m46] + o[m246]);
			k0 = 0.5 * (k3 + k4);

			if (k4 > k3)
			{
				s[m0] = 3;
			}
		}


}


void vvod(double* u, double* v,
	double* w, double* a, double* a1,
	double* mu, double* nu, double* o, double* o_max,
	double* h, int* s)
{
	int i, j, k, m;
	float buf;
	int m0, m2, m4, m6,
		m24, m26, m46, m246;

	double k0, k1, k2;
	//------------------------------------------------------------------------------------------------
	//								ввод данных(начало)
	//------------------------------------------------------------------------------------------------

	//------------------------------------------------------------------------------------------------
	//								начальные условия
	//------------------------------------------------------------------------------------------------

	for (m0 = 0;m0 < n3d;m0++)
	{
		u[m0] = 0;
		v[m0] = 0;
		w[m0] = 0;
		a[m0] = 0;
		mu[m0] = 1000;
		nu[m0] = 0.02;
	}

	for (j = 0;j < Ny;j++)
		for (i = 0;i < Nx;i++)
		{
			m = j + Ny * i;
			a1[m] = 0;

		}

	printf("nul OK\n");


	FILE* outs1;
	FILE* outs2;
	FILE* outs3;
	FILE* outs4;
	FILE* outs5;

	outs1 = fopen("setka1000.txt", "r+");
	outs2 = fopen("u.txt", "r+");
	outs3 = fopen("v.txt", "r+");
	outs4 = fopen("w.txt", "r+");
	outs5 = fopen("a.txt", "r+");
	

	for (k = 0;k < Nz;k++)
		for (j = 0;j < Ny;j++)
			for (i = 0;i < Nx;i++)
			{
				m0 = k + Nz * j + Ny * Nz * i;

				o[m0] = 0;
				o_max[m0] = 0;

			}

	for (j = 0;j < Ny - 1;j++)
		for (i = 0;i < Nx - 1;i++)
		{
			m = j + Ny * i;

			fscanf(outs1, "%f", &buf);
			h[m] = buf;
		}

	for (k = 0;k < Nz;k++)
		for (j = 0;j < Ny;j++)
			for (i = 0;i < Nx;i++)
			{
				m0 = k + Nz * j + Ny * Nz * i;
				m = j + Ny * i;


				if (h[m] > 0)
				{
					o_max[m0] = 2 * h[m] + 6 - k;
					if (o_max[m0] > 1)
						o_max[m0] = 1;
					if (o_max[m0] < 0)
						o_max[m0] = 0;
					if (k < 6)
						o_max[m0] = 1;

					if (k > 5)
						o[m0] = o_max[m0];
				}

			}

	for (k = 1;k < Nz - 1;k++)
		for (i = 1;i < Nx - 1;i++)
			for (j = 1;j < Ny - 1;j++)
			{
				m0 = k + Nz * j + Ny * Nz * i;
				m2 = m0 - Ny * Nz;
				m4 = m0 - Nz;
				m6 = m0 - 1;
				m24 = m0 - Ny * Nz - Nz;
				m26 = m0 - Ny * Nz - 1;
				m46 = m0 - Nz - 1;
				m246 = m0 - Ny * Nz - Nz - 1;

				k1 = 0.25 * (o[m0] + o[m4] + o[m6] + o[m46]);
				k2 = 0.25 * (o[m2] + o[m24] + o[m26] + o[m246]);
				k0 = 0.5 * (k1 + k2);

				s[m0] = 0;
				if (k0 > 0)
					s[m0] = 1;


			}
	printf("h OK\n");

	for (k = 0;k < Nz;k++)
		for (j = 0;j < Ny;j++)
			for (i = 0;i < Nx;i++)
			{
				m0 = k + Nz * j + Ny * Nz * i;

				fscanf(outs2, "%f", &buf);
				//u[m0]=buf;
				fscanf(outs3, "%f", &buf);
				//v[m0]=buf;
				fscanf(outs4, "%f", &buf);
				//w[m0]=buf;
				fscanf(outs5, "%f", &buf);
				//a[m0]=buf;

			}
	fclose(outs1);
	fclose(outs2);
	fclose(outs3);
	fclose(outs4);
	fclose(outs5);

	//------------------------------------------------------------------------------------------------
	//								граничные условия(конец)
	//------------------------------------------------------------------------------------------------

	//------------------------------------------------------------------------------------------------
	//								ввод данных(конец)
	//------------------------------------------------------------------------------------------------
}

void vivod(double* u, double* v, double* w, double* a)
{
	int i, j, k, m0;

	FILE* uoutfile;
	FILE* voutfile;
	FILE* woutfile;
	FILE* aoutfile;

	uoutfile = fopen("u.txt", "w+");
	voutfile = fopen("v.txt", "w+");
	woutfile = fopen("w.txt", "w+");
	aoutfile = fopen("a.txt", "w+");

	for (k = 0;k < Nz;k++)
	{
		for (j = 0;j < Ny;j++)
		{
			for (i = 0;i < Nx;i++)
			{
				m0 = k + Nz * j + Ny * Nz * i;
				fprintf(uoutfile, "%f	", u[m0]);
				fprintf(voutfile, "%f	", v[m0]);
				fprintf(woutfile, "%f	", w[m0]);
				fprintf(aoutfile, "%f	", a[m0]);
			}
			fprintf(uoutfile, "\n");
			fprintf(voutfile, "\n");
			fprintf(woutfile, "\n");
			fprintf(aoutfile, "\n");
		}
	}
	fclose(uoutfile);
	fclose(voutfile);
	fclose(woutfile);
	fclose(aoutfile);

}

void firstfunc(double* u, double* v, double* w,
	double* mu, double* nu, double* o, int* s)
{
	double omega, sk;
	int i, j, k, it = 0;
	int m0, m1, m2, m3, m4, m5, m6,
		m24, m26, m46, m246;

	double k0, k1, k2, k3, k4, k5, k6;

	double* c0 = new double[n3d];
	double* c1 = new double[n3d];
	double* c2 = new double[n3d];
	double* c3 = new double[n3d];
	double* c4 = new double[n3d];
	double* c5 = new double[n3d];
	double* c6 = new double[n3d];

	double* f1 = new double[n3d];
	double* f2 = new double[n3d];
	double* f3 = new double[n3d];


	for (m0 = 0;m0 < n3d;m0++)
	{
		c1[m0] = 0;
		c2[m0] = 0;
		c3[m0] = 0;
		c4[m0] = 0;
		c5[m0] = 0;
		c6[m0] = 0;
	}
	printf("mass1 OK\n");
	//------------------------------------------------------------------------------------------------
	//								коэфициенты и правая часть
	//------------------------------------------------------------------------------------------------

	for (k = 1;k < Nz - 1;k++)
		for (i = 1;i < Nx - 1;i++)
			for (j = 1;j < Ny - 1;j++)
			{
				m0 = k + Nz * j + Ny * Nz * i;
				m1 = m0 + Ny * Nz;
				m2 = m0 - Ny * Nz;
				m3 = m0 + Nz;
				m4 = m0 - Nz;
				m5 = m0 + 1;
				m6 = m0 - 1;
				m24 = m0 - Ny * Nz - Nz;
				m26 = m0 - Ny * Nz - 1;
				m46 = m0 - Nz - 1;
				m246 = m0 - Ny * Nz - Nz - 1;

				k1 = 0.25 * (o[m0] + o[m4] + o[m6] + o[m46]);
				k2 = 0.25 * (o[m2] + o[m24] + o[m26] + o[m246]);
				k3 = 0.25 * (o[m0] + o[m2] + o[m6] + o[m26]);
				k4 = 0.25 * (o[m4] + o[m24] + o[m46] + o[m246]);
				k5 = 0.25 * (o[m0] + o[m2] + o[m4] + o[m24]);
				k6 = 0.25 * (o[m6] + o[m26] + o[m46] + o[m246]);
				k0 = 0.5 * (k1 + k2);

				c1[m0] = 0.5 * dt * (-(u[m1] + u[m0]) / 2 + (mu[m1] + mu[m0]) / dx) / dx;
				c2[m0] = 0.5 * dt * ((u[m2] + u[m0]) / 2 + (mu[m2] + mu[m0]) / dx) / dx;
				c3[m0] = 0.5 * dt * (-(v[m3] + v[m0]) / 2 + (mu[m3] + mu[m0]) / dy) / dy;
				c4[m0] = 0.5 * dt * ((v[m4] + v[m0]) / 2 + (mu[m4] + mu[m0]) / dy) / dy;
				c5[m0] = 0.5 * dt * (-(w[m5] + w[m0]) / 2 + (nu[m5] + nu[m0]) / dz) / dz;
				c6[m0] = 0.5 * dt * ((w[m6] + w[m0]) / 2 + (nu[m6] + nu[m0]) / dz) / dz;


				if ((c1[m0] < 0) && (k1 > 0))
				{
					c1[m0] = 0.5 * dt * ((mu[m1] + mu[m0]) / dx) / dx;
					c2[m0] = 0.5 * dt * ((u[m2] + u[m0]) + (mu[m2] + mu[m0]) / dx) / dx;
				}
				if ((c2[m0] < 0) && (k2 > 0))
				{
					c1[m0] = 0.5 * dt * (-(u[m1] + u[m0]) + (mu[m1] + mu[m0]) / dx) / dx;
					c2[m0] = 0.5 * dt * ((mu[m2] + mu[m0]) / dx) / dx;
				}
				if ((c3[m0] < 0) && (k3 > 0))
				{
					c3[m0] = 0.5 * dt * ((mu[m3] + mu[m0]) / dy) / dy;
					c4[m0] = 0.5 * dt * ((v[m4] + v[m0]) + (mu[m4] + mu[m0]) / dy) / dy;
				}
				if ((c4[m0] < 0) && (k4 > 0))
				{
					c3[m0] = 0.5 * dt * (-(v[m3] + v[m0]) + (mu[m3] + mu[m0]) / dy) / dy;
					c4[m0] = 0.5 * dt * ((mu[m4] + mu[m0]) / dy) / dy;
				}
				if ((c5[m0] < 0) && (k5 > 0))
				{
					c5[m0] = 0.5 * dt * ((nu[m5] + nu[m0]) / dz) / dz;
					c6[m0] = 0.5 * dt * ((w[m6] + w[m0]) + (nu[m6] + nu[m0]) / dz) / dz;
				}
				if ((c6[m0] < 0) && (k6 > 0))
				{
					c5[m0] = 0.5 * dt * (-(w[m5] + w[m0]) + (nu[m5] + nu[m0]) / dz) / dz;
					c6[m0] = 0.5 * dt * ((nu[m6] + nu[m0]) / dz) / dz;
				}

				c1[m0] = k1 * c1[m0];
				c2[m0] = k2 * c2[m0];
				c3[m0] = k3 * c3[m0];
				c4[m0] = k4 * c4[m0];
				c5[m0] = k5 * c5[m0];
				c6[m0] = k6 * c6[m0];

				c0[m0] = k0 + c1[m0] + c3[m0] + c5[m0] + c2[m0] + c4[m0] + c6[m0];

				f1[m0] = k0 * (u[m0] + dt * kor * (v[m0] - w[m0]));
				f2[m0] = k0 * (v[m0] - dt * kor * u[m0]);
				f3[m0] = k0 * (w[m0] + dt * kor * u[m0]);

				if (k5 > k6)
				{
					sk = dt * ca * sqrt((sk_u - u[m0]) * (sk_u - u[m0]) + (sk_v - v[m0]) * (sk_v - v[m0]) + w[m0] * w[m0]);
					f1[m0] = f1[m0] + (k5 - k6) * sk * (sk_u) / dz;
					f2[m0] = f2[m0] + (k5 - k6) * sk * (sk_v) / dz;
					c0[m0] = c0[m0] + (k5 - k6) * sk / dz;

				}

				if (k6 > k5)
				{
					sk = dt * cv * sqrt(u[m0] * u[m0] + v[m0] * v[m0] + w[m0] * w[m0]) / dz;
					c0[m0] = c0[m0] + (k6 - k5) * sk;
				}

				if (s[m0] == 2)
				{
					c1[m0] = 0;
					c2[m0] = 0;
					c3[m0] = 0;
					c4[m0] = 0;
					c5[m0] = 0;
					c6[m0] = 0;


					c0[m0] = 1;

					f1[m0] = u[m0];
					f2[m0] = v[m0];
					f3[m0] = w[m0];

				}


			}
	printf("func1 OK\n");
	omega = 0;

	ptmss(u, c1, c2, c3, c4, c5, c6, c0, f1, s, omega, ev);
	ptmss(v, c1, c2, c3, c4, c5, c6, c0, f2, s, omega, ev);
	ptmss(w, c1, c2, c3, c4, c5, c6, c0, f3, s, omega, ev);

	delete[] c0;
	delete[] c1;
	delete[] c2;
	delete[] c3;
	delete[] c4;
	delete[] c5;
	delete[] c6;

	delete[] f1;
	delete[] f2;
	delete[] f3;
}



/// <summary>
/// метод прогонки вправо, 
/// метод решения СЛАУ 3х диагональной матрицы
/// </summary>
/// <param name="r"></param>
/// <param name="d0">коэффциент при В0</param>
/// <param name="d5">коэффциент при В5</param>
/// <param name="d6">коэффциент при В6</param>
/// <param name="f">правая часть</param>
/// <param name="c2">В2</param>
/// <param name="c4">В4</param>
/// <param name="omega"></param>
void sweep_right(double* r, double * d0, double * d5, double * d6, double* f, double* c2, double* c4, double omega)
{
	double* alf = new double[Nz];
	double* bet = new double[Nz];
	int m0, m2, m4, m5, m6, i, j, k;

	double k0, k1, k2, k3, k4, k5, k6, zn;
	bool flag = true;

	for (k = 0; k < Nz; k++) { alf[k] = bet[k] = 0; }

	for (i = 1;i < Nx; i++)
		for (j = 1; j < Ny; j++)
		{			
			alf[0] = 0;
			bet[0] = 0;
			for (k = 1; k < Nz; k++)
			{
				m0 = k + Nz * j + Ny * Nz * i;
				m2 = m0 - Ny * Nz;
				m4 = m0 - Nz;
				m6 = m0 - 1;

				f[m0] = r[m0] + omega * (c2[m0] * r[m2] + c4[m0] * r[m4]);

				zn = d0[m0] - alf[k - 1] * d6[m0];

				if (zn != 0)
				{
					alf[k] = d5[m0] / zn;
					bet[k] = (f[m0] + bet[k - 1] * d6[m0]) / zn;
				}
				//else
				//{
				//	cout << "\n ERROR! zn = 0 \n";
				//	return;
				//}
			}
			k = Nz - 1;
			m0 = k + Nz * j + Ny * Nz * i;
			r[m0] = bet[k];

			for (k = Nz - 2; k > 0; k--)
			{
				m0 = k + Nz * j + Ny * Nz * i;
				m5 = m0 + 1;

				if (d0[m0] > 0)
					r[m0] = bet[k] + alf[k] * r[m5];
			}

		}

	delete[] alf;
	delete[] bet;
}

/// <summary>
/// метод прогонки влево, 
/// метод решения СЛАУ 3х диагональной матрицы
/// </summary>
/// <param name="r"></param>
/// <param name="d0">коэффциент при В0</param>
/// <param name="d5">коэффциент при В5</param>
/// <param name="d6">коэффциент при В6</param>
/// <param name="f">правая часть</param>
/// <param name="c0">центральный узел</param>
/// <param name="c1">В3</param>
/// <param name="c3">В4</param>
/// <param name="omega"></param>
void sweep_left(double* r, double* d0, double* d5, double* d6, double* f, double* c0, double* c1, double* c3, double omega)
{
	double* alf = new double[Nz];
	double* bet = new double[Nz];
	int m0, m1, m3, m5, m6, i, j, k;

	double k0, k1, k2, k3, k4, k5, k6, zn;
	bool flag = true;

	for (k = 0; k < Nz; k++) { alf[k] = bet[k] = 0; }

	for (i = Nx - 2; i > 0; i--)
		for (j = Ny - 2; j > 0; j--)
		{
			alf[0] = 0;
			bet[0] = 0;
			for (k = 1; k < Nz; k++)
			{
				m0 = k + Nz * j + Ny * Nz * i;
				m1 = m0 + Ny * Nz;
				m3 = m0 + Nz;
				m5 = m0 + 1;

				f[m0] = r[m0] * c0[m0] + omega * (c1[m0] * r[m1] + c3[m0] * r[m3]);

				zn = d0[m0] - alf[k - 1] * d6[m0];

				if (zn != 0)
				{
					alf[k] = d5[m0] / zn;
					bet[k] = (f[m0] + bet[k - 1] * d6[m0]) / zn;
				}
				//else
				//{
				//	cout << "\n ERROR! zn = 0 \n";
				//	return;
				//}
			}
			k = Nz - 1;
			m0 = k + Nz * j + Ny * Nz * i;
			r[m0] = bet[k];

			for (k = Nz - 2; k > 0; k--)
			{
				m0 = k + Nz * j + Ny * Nz * i;
				m5 = m0 + 1;

				if (c0[m0] > 0)
					r[m0] = bet[k] + alf[k] * r[m5];
			}

		}

	delete[] alf;
	delete[] bet;
}

//------------------------------------------------------------------------------------------------
//									   333
//									  3   3		 Расчет
//									      3		скорости 
//									     3		на следующем
//										  3		временом шаге.
//								      3	  3		(явная схема)
//									   333
//------------------------------------------------------------------------------------------------

void ptmss(double* u, double* c1, double* c2, double* c3, double* c4,
	double* c5, double* c6, double* c0,
	double* f, int* s, double omega, double e)
{
	double max;
	double tay;
	int i, j, k, it = 0;
	int m0, m1, m2, m3, m4, m5, m6;


	tay = 2 * omega;
	double RwRw, ww, Aww, Awr, Rr;


	double* r = new double[n3d];
	double* wr = new double[n3d];
	double* d5 = new double[n3d]; //коэффициент при r5
	double* d6 = new double[n3d]; //коэффициент при r6
	double* d0 = new double[n3d]; //коэффициент при r0
	double* r1 = new double[n3d]; //правая часть f

	printf("ptmss start OK\n");
	//------------------------------------------------------------------------------------------------
	//								Метод попеременых треугольников(начало)
	//------------------------------------------------------------------------------------------------
	for (m0 = 0;m0 < n3d;m0++)
	{
		r[m0] = 0;
		d5[m0] = 0;
		d6[m0] = 0;
		d0[m0] = 0;
		r1[m0] = 0;
	}

	do
	{
		//------------------------------------------------------------------------------------------------
		//								Невязка
		//------------------------------------------------------------------------------------------------

		Aww = 0;
		RwRw = 0;
		ww = 0;
		max = 0;

		for (k = 1;k < Nz - 1;k++)
			for (i = 1;i < Nx - 1;i++)
				for (j = 1;j < Ny - 1;j++)
				{
					m0 = k + Nz * j + Ny * Nz * i;
					if (c0[m0] > 0)
					{
						m1 = m0 + Ny * Nz;
						m2 = m0 - Ny * Nz;
						m3 = m0 + Nz;
						m4 = m0 - Nz;
						m5 = m0 + 1;
						m6 = m0 - 1;


						r[m0] = f[m0] - c0[m0] * u[m0] + (c1[m0] * u[m1] + c2[m0] * u[m2]
							+ c3[m0] * u[m3] + c4[m0] * u[m4] + c5[m0] * u[m5] + c6[m0] * u[m6]);
						if (max < fabs(r[m0])) { max = fabs(r[m0]); };
					}
				}

		//------------------------------------------------------------------------------------------------
		//								Итерация поперемено треугольным методом
		//------------------------------------------------------------------------------------------------


		for (k = 1;k < Nz - 1;k++)
			for (i = 1;i < Nx - 1;i++)
				for (j = 1;j < Ny - 1;j++)
				{
					m0 = k + Nz * j + Ny * Nz * i;
					if (c0[m0] > 0)
					{
						m2 = m0 - Ny * Nz;
						m4 = m0 - Nz;
						m6 = m0 - 1;

						d6[m0] = 0.5 * omega * c6[m0];
						d0[m0] = (0.5 * omega + 1) * c0[m0];
						d5[m0] = 0.5 * omega * c5[m0];
						//r[m0] = (omega * (c2[m0] * r[m2] + c4[m0] * r[m4] + c6[m0] * r[m6]) + r[m0]) / ((0.5 * omega + 1) * c0[m0]);
					}
				}
		//- 0.5 * omega * c6[m0] * r[m6] + (0.5 * omega + 1) * c0[m0] * r[m0]  - 0.5 * omega * c5[m0] * r[m5] = r[m0] + omega * (c2[m0] * r[m2] + c4[m0] * r[m4]);
		sweep_right(r, d0, d5, d6, r1, c2, c4, omega);

		for (k = Nz - 2;k > 0;k--)
			for (i = Nx - 2;i > 0;i--)
				for (j = Ny - 2;j > 0;j--)
				{
					m0 = k + Nz * j + Ny * Nz * i;
					if (c0[m0] > 0)
					{
						m1 = m0 + Ny * Nz;
						m3 = m0 + Nz;
						m5 = m0 + 1;

						d6[m0] = 0.5 * omega * c6[m0];
						d0[m0] = (0.5 * omega + 1) * c0[m0];
						d5[m0] = 0.5 * omega * c5[m0];						
						//r[m0] = (omega * (c1[m0] * r[m1] + c3[m0] * r[m3] + c5[m0] * r[m5]) + r[m0] * c0[m0]) / ((0.5 * omega + 1) * c0[m0]);
					}
				}
		//- 0.5 * omega * c6[m0] * r[m6] + (0.5 * omega + 1) * c0[m0] * r[m0]  - 0.5 * omega * c5[m0] * r[m5] = r[m0] * c0[m0] + omega * (c1[m0] * r[m1] + c3[m0] * r[m3]);
		sweep_left(r, d0, d5, d6, r1, c0, c1, c3, omega);

		for (k = 1;k < Nz - 1;k++)
			for (i = 1;i < Nx - 1;i++)
				for (j = 1;j < Ny - 1;j++)
				{
					m0 = k + Nz * j + Ny * Nz * i;
					if (c0[m0] > 0)
					{
						m1 = m0 + Ny * Nz;
						m2 = m0 - Ny * Nz;
						m3 = m0 + Nz;
						m4 = m0 - Nz;
						m5 = m0 + 1;
						m6 = m0 - 1;

						Awr = 1.1 * c0[m0] * r[m0] - (c1[m0] * r[m1] + c3[m0] * r[m3] + c5[m0] * r[m5] + c2[m0] * r[m2] + c4[m0] * r[m4] + c6[m0] * r[m6]);
						Rr = 0.5 * c0[m0] * r[m0] - (c1[m0] * r[m1] + c3[m0] * r[m3] + 0.5 * c5[m0] * r[m5] + 0.5 * c6[m0] * r[m6]);

						RwRw = RwRw + Rr * Rr / c0[m0];
						Aww = Aww + Awr * r[m0];
						ww = ww + c0[m0] * r[m0] * r[m0];

					}
				}

		if (ww > 0)
		{
			tay = 2 * omega + ww / Aww;
			omega = sqrt(ww / RwRw);

		}

		//------------------------------------------------------------------------------------------------
		//								шаг ПТМ
		//------------------------------------------------------------------------------------------------


		for (m0 = 0;m0 < n3d;m0++)
			u[m0] = u[m0] + 1 * tay * r[m0];

		it++;

		printf("%f	%f	%f	%f	%d\n", t, max * 1000000, omega, tay, it);
	} while ((max > e) && (it < 300));

	printf("ptmss OK\n");

	delete[] r;
	delete[] r1;
	delete[] d5;
	delete[] d6;
	delete[] d0;
	delete[] wr;

	//------------------------------------------------------------------------------------------------
	//								Метод попеременых треугольников(конец)
	//------------------------------------------------------------------------------------------------
}