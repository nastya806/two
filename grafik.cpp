// Ялунина Анастасия, МИ-1701
#include <fstream>
#include <vector>
#include <cstdint>
#include <cmath>
constexpr uint16_t weight = 1920, height = 1080;
constexpr double sx = 35, sy = 35, sz = -350, s1 = 0.6, s2 = 0.02, x_min = -15,
		x_max = 15, y_min = -15, y_max = 15; //s1-интервал волн
void plot(std::vector<uint32_t>& p,  std::vector<int>&h, int x, int y, uint32_t c) {
	if (x < 0 || x >= weight)
		return;
//	if (y < 0 || y >= height)
//		return;
	if (y>=h[x]) return;
	h[x]=y;
	if(y<0) return;
	p[y * weight + x] = c;
}
#pragma pack(push, 1)
struct TGA {
	uint8_t id_len; // Длина идентификатора
	uint8_t pal_type; // Тип палитры
	uint8_t img_type; // Тип изображения
	uint8_t pal_desc[5]; // Описание палитры
	uint16_t xpos, ypos; // Положение по оси X,Y
	uint16_t weight, height; // Ширина, высота
	uint8_t depth; // Глубина цвета
	uint8_t img_desc; // Оптсатель изображения
};
#pragma pack (pop)
double sinc(double x) {
	if (x == 0)
		return 1.;
	return sin(x) / x;
}
int main() {
	TGA tga {};
	std::vector<uint32_t> img;
	img.resize(weight * height);
	std::vector<int>har;
	har.resize(weight);
	for(auto&&a : har) a=height;
	for (int i = 0; i< 1920*1080;i++) img[i] = 0x000000;//цвет фона
	for (double x = x_max; x >= x_min; x -= s1) {
		for (double y = y_max; y >= y_min; y -= s2) {
			double z = sinc(sqrt(x * x + y * y));
			int rx = int(
					weight / 2 - sx * x * cos(M_PI / 6) + sy * y * sin(M_PI / 6));
			int ry = int(
					height / 2 + sx * x * sin(M_PI / 6) + sy * y * sin(M_PI / 6)
							- sz * z);
			plot(img, har ,rx, ry, 	0xFF0000);
		}
	}
	for(auto&&a : har) a=height;
	for (double y = y_max; y >= y_min; y -= s1) {
		for (double x = x_max; x >= x_min; x -= s2) {
			double z = sinc(sqrt(x * x + y * y));
			int rx = int(
					weight / 2 - sx * x * cos(M_PI / 6) + sy * y * sin(M_PI / 6));
			int ry = int(
					height / 2 + sx * x * sin(M_PI / 6) + sy * y * sin(M_PI / 6)
							- sz * z);
			plot(img, har,  rx, ry, 0x00FF00);
		}
	}

	std::fstream f;
	tga.img_type = 2;
	tga.weight = 1920;
	tga.height = 1080;
	tga.depth = 32;
	tga.img_desc = 0x28;
	f.open("grafik.tga", std::ios::out | std::ios::binary);
	f.write(reinterpret_cast<char*>(&tga), sizeof(TGA));
	f.write(reinterpret_cast<char*>(&img[0]), 1920 * 1080 * 4);

}
