#include "stdafx.h"
#include <iostream> 
#include <opencv2/opencv.hpp> 
using namespace std;
using namespace cv;
int main()
{
	// inisialisasi variabel untuk akses image 
	int height, width, step, channels;
	uchar* data1;
	int i, j;
	int hue2; //hue index 
	float r, g, b, wmax, wmin, d, hue, hmax; //RGB to Hue variable 
	float h[3][30]; //histogram feature  training 
	float gn[30]; //general feature training 
	float f[3][30]; //specitic feature  training 
	float dd, dmin; //distance matching  feature 
	int klasifikasi; //category 
	float h_uji[3][30]; //histogram capture test  picture 
	float f_uji[3][30]; //feature test object  
	CvPoint pt1, pt2; //point x and y position 
	CvFont font; //font initialize 
	IplImage* hist3 = cvCreateImage(cvSize(640, 600), 8, 3);
	IplImage* source = 0;
	// reset histogram 
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 30; j++) {
			h[i][j] = 0;
			f[i][j] = 0;
			h_uji[i][j] = 0;
			f_uji[i][j] = 0;
		}
	}
	//------------------- Tomat Hijau ------------------- 
	// untuk loading tomat hijau 
	char* imageName = "hijau.jpg";
	IplImage* src = cvLoadImage(imageName, -1);
	height = src->height;
	width = src->width;
	step = src->widthStep;
	channels = src->nChannels;
	data1 = (uchar*)src->imageData;
	// mengakses warna r,g,b tiap pixel di gambar, diubah ke HSV manual 
	// RGB ke HSV dengan menggunakan rumus yang ada pada paper referensi 
	for (i = 0; i < src->height; i++) {
		for (j = 0; j < src->width; j++) {
			b = data1[i * step + j * channels + 0]; //blue 0-255 
			g = data1[i * step + j * channels + 1]; //green 0-255 
			r = data1[i * step + j * channels + 2]; //red 0-255 
			// mencari nilai maksimum antara nilai r,g,b, simpan di wmax 
			wmax = r;
			if (g > wmax) wmax = g;
			if (b > wmax) wmax = b;
			// mencari nilai minimum antara nilai r,g,b, simpan di wmin 
			wmin = r;
			if (g < wmin) wmin = g;
			if (b < wmin) wmin = b;
			// cari nilai jarak = d = wmax-wmin 
			d = wmax - wmin;
			// ubah ke nilai HUE berdasar cara konversi dari RGB ke HUE, buat Index
			if (d == 0)
				hue = 0;
			else
			{
				if (wmax == r) hue = (float)(60 * ((g - b) / d));
				if (wmax == g) hue = (float)(60 * (((b - r) / d) +
					2));
				if (wmax == b) hue = (float)(60 * (((r - g) / d) +
					4));
				if (hue < 0) hue = hue + 360;
				// membuat index sebanyak 30 index (12 derajat tiap index)
				hue2 = (int)(hue / 12);
			}
			// disimpan nilai index hue ke dalam array untuk histogram hueindex
			h[0][hue2] = h[0][hue2] + 1;
		}
	}
	// End of Hue Index 
	// Normalisasi Histogram Hue Index 
	hmax = h[0][0];
	for (i = 0; i < 30; i++) {
		if (h[0][i] > hmax)
			hmax = h[0][i];
	}
	for (i = 0; i < 30; i++) {
		h[0][i] = h[0][i] / hmax;
	}
	// End of Normalisasi 
	// ------------------ End of Tomat Hijau ------------- 
	//------------------- Tomat Campur ------------------- 
	// untuk loading tomat campur 
	char* imageName2 = "campur.jpg";
	IplImage* src2 = cvLoadImage(imageName2, -1);
	height = src2->height;
	width = src2->width;
	step = src2->widthStep;
	channels = src2->nChannels;
	data1 = (uchar*)src2->imageData;
	// mengakses warna r,g,b tiap pixel di gambar, diubah ke HSV manual 
	// RGB ke HSV dengan menggunakan rumus yang ada pada paper referensi 
	for (i = 0; i < src2->height; i++) {
		for (j = 0; j < src2->width; j++) {
			b = data1[i * step + j * channels + 0]; //blue 0-255 
			g = data1[i * step + j * channels + 1]; //green 0-255 
			r = data1[i * step + j * channels + 2]; //red 0-255 
			// mencari nilai maksimum antara nilai r,g,b, simpan di wmax 
			wmax = r;
			if (g > wmax) wmax = g;
			if (b > wmax) wmax = b;
			// mencari nilai minimum antara nilai r,g,b, simpan di wmin 
			wmin = r;
			if (g < wmin) wmin = g;
			if (b < wmin) wmin = b;
			// cari nilai jarak = d = wmax-wmin 
			d = wmax - wmin;
			// ubah ke nilai HUE berdasar cara konversi dari RGB ke HUE, dan buat Index
			if (d == 0)
				hue = 0;
			else
			{
				if (wmax == r) hue = (float)(60 * ((g - b) / d));
				if (wmax == g) hue = (float)(60 * (((b - r) / d) +
					2));
				if (wmax == b) hue = (float)(60 * (((r - g) / d) +
					4));
				if (hue < 0) hue = hue + 360;
				// membuat index sebanyak 30 index (12 derajat tiap index)
				hue2 = (int)(hue / 12);
			}
			// disimpan nilai index hue ke dalam array untuk histogram hue index
			h[1][hue2] = h[1][hue2] + 1;
		}
	}
	// End of Hue Index 
	// --------------- End of Tomat Campur ------------- 
	//------------------- Tomat Merah ------------------ 
	// untuk loading tomat merah 
	char* imageName3 = "merah.jpg";
	IplImage* src3 = cvLoadImage(imageName3, -1);
	height = src3->height;
	width = src3->width;
	step = src3->widthStep;
	channels = src3->nChannels;
	data1 = (uchar*)src3->imageData;
	// mengakses warna r,g,b tiap pixel di gambar, diubah ke HSV manual 
	// RGB ke HSV dengan menggunakan rumus yang ada pada paper referensi 
	for (i = 0; i < src3->height; i++) {
		for (j = 0; j < src3->width; j++) {
			b = data1[i * step + j * channels + 0]; //blue 0-255 
			g = data1[i * step + j * channels + 1]; //green 0-255 
			r = data1[i * step + j * channels + 2]; //red 0-255 
			// mencari nilai maksimum antara nilai r,g,b, simpan di wmax 
			wmax = r;
			if (g > wmax) wmax = g;
			if (b > wmax) wmax = b;
			// mencari nilai minimum antara nilai r,g,b, simpan di wmin 
			wmin = r;
			if (g < wmin) wmin = g;
			if (b < wmin) wmin = b;
			// cari nilai jarak = d = wmax-wmin 
			d = wmax - wmin;
			// ubah ke nilai HUE berdasar cara konversi dari RGB ke HUE, dan buat Index
			if (d == 0)
				hue = 0;
			else
			{
				if (wmax == r) hue = (float)(60 * ((g - b) / d));
				if (wmax == g) hue = (float)(60 * (((b - r) / d) + 2));
				if (wmax == b) hue = (float)(60 * (((r - g) / d) + 4));
				if (hue < 0) hue = hue + 360;
				// membuat index sebanyak 30 index (12 derajat tiap	index)
				hue2 = (int)(hue / 12);
			}
			// disimpan nilai index hue ke dalam array untuk histogram hue index
			h[2][hue2] = h[2][hue2] + 1;
		}
	}
	// End of Hue Index
	// Normalisasi Histogram Hue Index 
	hmax = h[2][0];
	for (i = 0; i < 30; i++) {
		if (h[2][i] > hmax)
			hmax = h[2][i];
	}
	for (i = 0; i < 30; i++) {
		h[2][i] = h[2][i] / hmax;
	}
	// End of Normalisasi 
	// --------------- End of Tomat Merah ------------- 
	// ----------------HISOGRAM INTERSEKSI ------------ 
	// Histogram General => dengan Histogram Interseksi 
	for (i = 0; i < 30; i++)
		gn[i] = 0;
	for (i = 0; i < 30; i++) {
		gn[i] = h[0][i];
		if (h[1][i] < gn[i]) gn[i] = h[1][i];
		if (h[2][i] < gn[i]) gn[i] = h[2][i];
	}
	// Histogram Fitur Spesifik Tomat - Fitur Spesifik untuk Klasifikasi 
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 30; j++) {
			f[i][j] = fabs(h[i][j] - gn[j]);
		}
	}
	// -------------------------------------------------------------------- 
	// Menggambar grafik HISTOGRAM HU INDEX dan GENERAL FEATURE 
	// Menggambar histogram dengan fungsi line dengan ketebalan tertentu 
	IplImage* hist1 = cvCreateImage(cvSize(640, 600), 8, 3);
	// sumbu x dan y tomat 1 
	pt1.x = 10; pt1.y = 100; pt2.x = 630; pt2.y = 100;
	cvLine(hist1, pt1, pt2, CV_RGB(255, 0, 0), 3, CV_AA, 0); //sumbu x pt1.x = 10; pt1.y = 0; pt2.x = 10; pt2.y = 100; 
	cvLine(hist1, pt1, pt2, CV_RGB(255, 0, 0), 3, CV_AA, 0); //sumbu y pt1.x = 300; pt1.y = 60; 
	cvInitFont(&font, CV_FONT_HERSHEY_COMPLEX, 0.5, 0.5, 0.0, 1, CV_AA); cvPutText(hist1, "Tomat Hijau", pt1, &font, CV_RGB(0, 0, 255)); // histogram tomat 1 
	for (i = 1; i < 30; i++) {
		pt1.x = 10 + 20 * i;
		pt1.y = 100 - (int)(h[0][i] * 100);
		pt2.x = 10 + 20 * i;
		pt2.y = 100;
		cvLine(hist1, pt1, pt2, CV_RGB(255, 255, 0), 3, CV_AA, 0);
	}
	// sumbu x dan y tomat 2 
	pt1.x = 10; pt1.y = 250; pt2.x = 630; pt2.y = 250;
	cvLine(hist1, pt1, pt2, CV_RGB(255, 0, 0), 3, CV_AA, 0); //sumbu x pt1.x = 10; pt1.y = 0; pt2.x = 10; pt2.y = 250; 
	cvLine(hist1, pt1, pt2, CV_RGB(255, 0, 0), 3, CV_AA, 0); //sumbu y pt1.x = 300; pt1.y = 200; 
	cvInitFont(&font, CV_FONT_HERSHEY_COMPLEX, 0.5, 0.5, 0.0, 1, CV_AA); cvPutText(hist1, "Tomat Campur", pt1, &font, CV_RGB(0, 0, 255));
	// histogram tomat 2 
	for (i = 1; i < 30; i++) {
		pt1.x = 10 + 20 * i;
		pt1.y = 250 - (int)(h[1][i] * 100);
		pt2.x = 10 + 20 * i;
		pt2.y = 250;
		cvLine(hist1, pt1, pt2, CV_RGB(255, 255, 0), 3, CV_AA, 0);
	}
	// sumbu x dan y tomat 3 
	pt1.x = 10; pt1.y = 400; pt2.x = 630; pt2.y = 400;
	cvLine(hist1, pt1, pt2, CV_RGB(255, 0, 0), 3, CV_AA, 0); //sumbu x pt1.x = 10; pt1.y = 0; pt2.x = 10; pt2.y = 400; 
	cvLine(hist1, pt1, pt2, CV_RGB(255, 0, 0), 3, CV_AA, 0); //sumbu y pt1.x = 300; pt1.y = 360; 
	cvInitFont(&font, CV_FONT_HERSHEY_COMPLEX, 0.5, 0.5, 0.0, 1, CV_AA); cvPutText(hist1, "Tomat Merah", pt1, &font, CV_RGB(0, 0, 255)); // histogram tomat 3 
	for (i = 1; i < 30; i++) {
		pt1.x = 10 + 20 * i;
		pt1.y = 400 - (int)(h[2][i] * 100);
		pt2.x = 10 + 20 * i;
		pt2.y = 400;
		cvLine(hist1, pt1, pt2, CV_RGB(255, 255, 0), 3, CV_AA, 0);
	}
	// sumbu x dan y Histogram General Tomat 1,2,3 
	pt1.x = 10; pt1.y = 550; pt2.x = 630; pt2.y = 550;
	cvLine(hist1, pt1, pt2, CV_RGB(255, 0, 0), 3, CV_AA, 0); //sumbu x pt1.x = 10; pt1.y = 0; pt2.x = 10; pt2.y = 550; 
	cvLine(hist1, pt1, pt2, CV_RGB(255, 0, 0), 3, CV_AA, 0); //sumbu y pt1.x = 300; pt1.y = 510; 
	cvInitFont(&font, CV_FONT_HERSHEY_COMPLEX, 0.5, 0.5, 0.0, 1, CV_AA); cvPutText(hist1, "Histogram General Tomat", pt1, &font, CV_RGB(0, 0, 255)); // histogram General Tomat - Interseksi 
	for (i = 1; i < 30; i++) {
		pt1.x = 10 + 20 * i;
		pt1.y = 550 - (int)(gn[i] * 100);
		pt2.x = 10 + 20 * i;
		pt2.y = 550;
		cvLine(hist1, pt1, pt2, CV_RGB(255, 255, 0), 3, CV_AA, 0);
	}
	// gambar hist obyek Tomat - Hijau - Campur - Merah 
	cvNamedWindow("Hist Hue Index Tomat dan Histogram Interseksi", 1);
	cvShowImage("Hist Hue Index Tomat dan Histogram Interseksi", hist1);
	// END OF HIST 1 ----------------------------------------------------------- 
	// ------------------------------------------------------------------------- 
	// Menggambar grafik HISOGRAM SPECIFIC FEATURE DAN GENERAL FEATURE 
	// Menggambar histogram dengan fungsi line dengan ketebalan tertentu 
	IplImage* hist2 = cvCreateImage(cvSize(640, 600), 8, 3);
	// sumbu x dan y tomat 1 
	pt1.x = 10; pt1.y = 100; pt2.x = 630; pt2.y = 100;
	cvLine(hist2, pt1, pt2, CV_RGB(255, 0, 0), 3, CV_AA, 0); //sumbu x pt1.x = 10; pt1.y = 0; pt2.x = 10; pt2.y = 100; 
	cvLine(hist2, pt1, pt2, CV_RGB(255, 0, 0), 3, CV_AA, 0); //sumbu y pt1.x = 300; pt1.y = 60; 
	cvInitFont(&font, CV_FONT_HERSHEY_COMPLEX, 0.5, 0.5, 0.0, 1, CV_AA); cvPutText(hist2, "Hist Fitur Spesifik Tomat Hijau", pt1, &font, CV_RGB(0, 0, 255));
	// histogram tomat 1 
	for (i = 1; i < 30; i++) {
		pt1.x = 10 + 20 * i;
		pt1.y = 100 - (int)(f[0][i] * 100);
		pt2.x = 10 + 20 * i;
		pt2.y = 100;
		cvLine(hist2, pt1, pt2, CV_RGB(255, 255, 0), 3, CV_AA, 0);
	}
	// sumbu x dan y tomat 2 
	pt1.x = 10; pt1.y = 250; pt2.x = 630; pt2.y = 250;
	cvLine(hist2, pt1, pt2, CV_RGB(255, 0, 0), 3, CV_AA, 0); //sumbu x pt1.x = 10; pt1.y = 0; pt2.x = 10; pt2.y = 250; 
	cvLine(hist2, pt1, pt2, CV_RGB(255, 0, 0), 3, CV_AA, 0); //sumbu y pt1.x = 300; pt1.y = 200; 
	cvInitFont(&font, CV_FONT_HERSHEY_COMPLEX, 0.5, 0.5, 0.0, 1, CV_AA); cvPutText(hist2, "Hist Fitur Spesifik Tomat Campur", pt1, &font, CV_RGB(0, 0, 255));
	// histogram tomat 2 
	for (i = 1; i < 30; i++) {
		pt1.x = 10 + 20 * i;
		pt1.y = 250 - (int)(f[1][i] * 100);
		pt2.x = 10 + 20 * i;
		pt2.y = 250;
		cvLine(hist2, pt1, pt2, CV_RGB(255, 255, 0), 3, CV_AA, 0);
	}
	// sumbu x dan y tomat 3 
	pt1.x = 10; pt1.y = 400; pt2.x = 630; pt2.y = 400;
	cvLine(hist2, pt1, pt2, CV_RGB(255, 0, 0), 3, CV_AA, 0); //sumbu x pt1.x = 10; pt1.y = 0; pt2.x = 10; pt2.y = 400; 
	cvLine(hist2, pt1, pt2, CV_RGB(255, 0, 0), 3, CV_AA, 0); //sumbu y pt1.x = 300; pt1.y = 360; 
	cvInitFont(&font, CV_FONT_HERSHEY_COMPLEX, 0.5, 0.5, 0.0, 1, CV_AA); cvPutText(hist2, "Hist Fitur Spesifik Tomat Merah", pt1, &font, CV_RGB(0, 0, 255));
	// histogram tomat 3 
	for (i = 1; i < 30; i++) {
		pt1.x = 10 + 20 * i;
		pt1.y = 400 - (int)(f[2][i] * 100);
		pt2.x = 10 + 20 * i;
		pt2.y = 400;
		cvLine(hist2, pt1, pt2, CV_RGB(255, 255, 0), 3, CV_AA, 0);
	}
	// sumbu x dan y Histogram General Tomat 1,2,3 
	pt1.x = 10; pt1.y = 550; pt2.x = 630; pt2.y = 550;
	cvLine(hist2, pt1, pt2, CV_RGB(255, 0, 0), 3, CV_AA, 0); //sumbu x pt1.x = 10; pt1.y = 0; pt2.x = 10; pt2.y = 550; 
	cvLine(hist2, pt1, pt2, CV_RGB(255, 0, 0), 3, CV_AA, 0); //sumbu y pt1.x = 300; pt1.y = 510; 
	cvInitFont(&font, CV_FONT_HERSHEY_COMPLEX, 0.5, 0.5, 0.0, 1, CV_AA); cvPutText(hist2, "Histogram General Tomat", pt1, &font, CV_RGB(0, 0, 255)); // histogram General Tomat - Interseksi 
	for (i = 1; i < 30; i++) {
		pt1.x = 10 + 20 * i;
		pt1.y = 550 - (int)(gn[i] * 100);
		pt2.x = 10 + 20 * i;
		pt2.y = 550;
		cvLine(hist2, pt1, pt2, CV_RGB(255, 255, 0), 3, CV_AA, 0);
	}
	// gambar hist obyek Tomat - Hijau - Campur - Merah 
	cvNamedWindow("Hist Fitur Spesifik Tomat", 1);
	cvShowImage("Hist Fitur Spesifik Tomat", hist2);
	// END OF HIST 2 ----------------------------------------------------------- --- 
	cvShowImage("Tomat Hijau", src);
	cvShowImage("Tomat Campur", src2);
	cvShowImage("Tomat Merah", src3);
	//uji data 
	for (;;)
	{
		source = cvLoadImage("campur.jpg", -1);
		cvShowImage("Uji", source);
		if (1 == source->origin) // 1 means the image is inverted 
		{
			cvFlip(source, 0, 0);
			source->origin = 0;
		}
		height = source->height;
		width = source->width;
		step = source->widthStep;
		channels = source->nChannels;
		data1 = (uchar*)source->imageData;
		// mengakses warna r,g,b tiap pixel di gambar, diubah ke HSV 
		// RGB ke HSV dengan menggunakan rumus yang ada di paper 
		for (i = 0; i < source->height; i++) {
			for (j = 0; j < source->width; j++) {
				b = data1[i * step + j * channels + 0];
				//blue 0-255 
				g = data1[i * step + j * channels + 1];
				//green 0-255 
				r = data1[i * step + j * channels + 2]; //red 0-255 
				// mencari nilai maksimum antara nilai r,g,b, 
				// simpan di wmax 
				wmax = r;
				if (g > wmax) wmax = g;
				if (b > wmax) wmax = b;
				// mencari nilai minimum antara nilai r,g,b, 
				// simpan di wmin 
				wmin = r;
				if (g < wmin) wmin = g;
				if (b < wmin) wmin = b;
				// cari nilai jarak = d = wmax-wmin 
				d = wmax - wmin;
				// ubah ke nilai HUE berdasar cara konversi dari 
				// RGB ke HUE , dan buat Index 
				if (d == 0)
					hue = 0;
				else
				{
					if (wmax == r) hue = (float)(60 * ((g - b) /
						d));
					if (wmax == g) hue = (float)(60 * (((b - r) / d) + 2));
					if (wmax == b) hue = (float)(60 * (((r - g) / d)
						+ 4));
					if (hue < 0) hue = hue + 360;
					// membuat index sebanyak 30 index (12 derajat 
					// tiap index) 
					hue2 = (int)(hue / 12);
				}
				// disimpan nilai index hue ke dalam array untuk 
				// histogram hue index 
				h_uji[0][hue2] = h_uji[0][hue2] + 1;
			}
		}
		// End of Hue Index 
		// Normalisasi Histogram Hue Index 
		hmax = h_uji[0][0];
		for (i = 0; i < 30; i++) {
			if (h_uji[0][i] > hmax)
				hmax = h_uji[0][i];
		}
		for (i = 0; i < 30; i++) {
			h_uji[0][i] = h_uji[0][i] / hmax;
		}
		// End of Normalisasi 
		// Histogram Fitur Spesifik Tomat - Fitur Spesifik untuk 
		// Klasifikasi 
		for (i = 0; i < 30; i++) {
			f_uji[0][i] = fabs(h_uji[0][i] - gn[i]);
		}
		// --------------- End of FITUR UJI DATA ------------- 
		// --------------- MATCHING FEATURE ------------------- 
		dd = 0;
		klasifikasi = 3;
		dmin = 10;
		for (i = 0; i < 3; i++) {
			dd = 0;
			for (j = 0; j < 30; j++) {
				dd = dd + fabs(f_uji[0][j] - f[i][j]);
			}
			dd = dd / 30;
			if (dd < dmin) {
				dmin = dd;
				klasifikasi = i;
			}
		}
		// ------------ END OF MATCHING FEATURE --------------- 
		// Menampilkan Hasil Klasifikasi di layar 
		if (klasifikasi == 0) {
			printf("Tomat HIJAU \n""\t\t\n");
		}
		if (klasifikasi == 1) {
			printf("Tomat CAMPUR \n""\t\t\n");
		}
		if (klasifikasi == 2) {
			printf("Tomat MERAH \n""\t\t\n");
		}
		//----------------------------- END OF UJI DATA ONLINE -------------- ---------------- 
		waitKey(0);
	}
}