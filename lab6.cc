#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <math.h>
#include <map>
#pragma warning("C4430")
using namespace std;
map <int, int> mes={ {1,31},{2,28},{3,31},{4,30},{5,31},{6,30},{7,31},{8,31},{9,30},{10,31},{11,30},{12,31} };
	int time_b(int i1, int i2,int y)
	{
		int t;

		for (int i = i1+1; i < i2; i++)
		{
			if (y % 4 == 0 && i == 2)
				t += 29 * 24 * 360;
			else
			t += mes[i] * 24 * 360;
		}
		return t;
	}
class Gps {
private:
	double lat, lon;
	string time;
	double ele;
	int first_y;
	bool flag = true;
public:
	// Широта
	double latitude() const { return lat; }

	// Долгота
	double longitude() const { return lon; }

	// Высота над уровнем моря
	double elevation() const { return ele; }

	// Расстояние до другой точки
	double distanceTo(const Gps& other)
	{
		double ss;
		ss = pow(lat-other.latitude(),2)+pow(lon-other.longitude(),2)+pow(abs(ele-other.elevation()),2);
		return ss;
	}
	void get() {
		printf("%f\n",lat);
	}
	void set(ifstream& f) {
		string dop;
		f >> lat >> lon >> time >> ele;
		if (flag) { dop = time.substr(0, 4); first_y = stoi(dop); flag = false; }
	}
	void get_f(ofstream& f) {
		f << lat << lon << time << ele;
	}
	bool check_t() {
		string dop;
		dop = time.substr(0, 4);
		int year = stoi(dop);
		if (year <= 0 || year < first_y) {
			return false;
			cout << "Неверный год!!\n";
		}
		else return true;
	}
	int get_t(const int l) {
		string dop;
		int dop1;
		switch (l){
		case 1: dop = time.substr(0, 4);
			dop1 = stoi(dop); return dop1;
		case 2:
			dop = time.substr(5, 2);
			dop1 = stoi(dop); return dop1;
		case 3: dop = time.substr(8, 2);
			dop1 = stoi(dop); return dop1;
		case 4: dop1 = stoi(time.substr(11, 2))*360+ stoi(time.substr(14, 2))*60+stoi(time.substr(17,2));
			 return dop1;
		default:cout << "Еблан?Нет, такой команды )))\n";

		}
	}
};
int main()
{
	vector <Gps> gps_list;
	Gps r;
	int l;
	ifstream file; ofstream ft;
	file.open("test.txt");
	if (!file) {
		 printf("ERORR!\n");
		//Обработка ошибки открытия файла
	}
	int i = 0;
	while (!file.eof()) {
		r.set(file);
		r.get();
		gps_list.push_back(r);
	}
	ft.open("test1.txt");
	double max_ele = gps_list[0].elevation(), min_ele = gps_list[0].elevation();
	double max_speed = 0;
	int time_m=0, time_r=0;
	double dist = 0,dop_dist=0;
	int dop_t = 0;
	double ele_u= gps_list[0].elevation(),ele_d=0;
	for (i = 1; i < gps_list.size(); i++) {
		dist += gps_list[i].distanceTo(gps_list[i-1]);
		dop_dist = gps_list[i].distanceTo(gps_list[i - 1]);
		if (gps_list[i].elevation() >= gps_list[i-1].elevation()) {
			ele_u += gps_list[1].elevation();
		}
		else {
			ele_u -= gps_list[i-1].elevation() - gps_list[i].elevation();
			ele_d+= gps_list[i-1].elevation() - gps_list[i].elevation();
		}
		if (gps_list[i].elevation() > max_ele)
			max_ele = gps_list[i].elevation();
		if (gps_list[i].elevation() < min_ele)
			min_ele = gps_list[i].elevation();
		if (gps_list[i].latitude() == gps_list[i - 1].latitude() && gps_list[i].longitude() == gps_list[i - 1].longitude())
			if (gps_list[i].get_t(1) == gps_list[i - 1].get_t(1)) {
				if (gps_list[i].get_t(2) == gps_list[i - 1].get_t(2)) {
					if (gps_list[i].get_t(3) == gps_list[i - 1].get_t(3)) {
						time_r += gps_list[i].get_t(4) - gps_list[i - 1].get_t(4);
						dop_t = gps_list[i].get_t(4) - gps_list[i - 1].get_t(4);
					}
					else {
						time_m += (gps_list[i].get_t(3) - gps_list[i - 1].get_t(3) - 1) * 24 * 360 + gps_list[i].get_t(4) + 24 * 360 - gps_list[i - 1].get_t(4);
						dop_t = (gps_list[i].get_t(3) - gps_list[i - 1].get_t(3) - 1) * 24 * 360 + gps_list[i].get_t(4) + 24 * 360 - gps_list[i - 1].get_t(4);
					}
				}
				else {
					dop_t = time_b(gps_list[i].get_t(2), gps_list[i - 1].get_t(2), gps_list[i].get_t(1)) + gps_list[i].get_t(4) + 24 * 360 - gps_list[i - 1].get_t(4);
					time_m+=time_b(gps_list[i].get_t(2), gps_list[i - 1].get_t(2), gps_list[i].get_t(1))+gps_list[i].get_t(4) + 24 * 360 - gps_list[i - 1].get_t(4);
					if (gps_list[i].get_t(1) % 4 == 0) {
						if (gps_list[i - 1].get_t(2) == 2) {
							time_m += (29 - gps_list[i - 1].get_t(2)) * 24 * 360;
							dop_t += (29 - gps_list[i - 1].get_t(2)) * 24 * 360;
						}
						else {
							time_m += (mes[i - 1] - gps_list[i - 1].get_t(2) * 24 * 360);
							dop_t += (mes[i - 1] - gps_list[i - 1].get_t(2) * 24 * 360);
						}
					}
					time_m += (gps_list[i].get_t(2) - 1)*24*360;
					dop_t+= (gps_list[i].get_t(2) - 1) * 24 * 360;
				}
			}
		if (dop_dist / dop_t > max_speed)
			max_speed = dop_dist;
	}
	ft << "Общее время в пути:"<<time_m+time_r<<"\nВремя в пути:"<<time_m<<"\nВремя отдыха:"<<time_r<<"\nМаксимальная скорость:"<<max_speed;
	ft << "\nОбщий набор высоты:" << ele_u << "\nОбщий спуск:" << ele_d<<"\nСредняя скорость:"<<dist/(time_m+time_r)<<"\nСредняя скорость движения:"<<dist/time_m;
	ft << "\nОбщаяя дистанция:" << dist << "\nОбщее время дней:" << (time_m+time_r)/360/24<<" часов:"<< (time_m + time_r) /360%24<<"минут:"<< (time_m + time_r) / 360 % 24 / 60 <<"секунд:"<< (time_m + time_r) / 360 % 24 % 60;
	file.close(); ft.close();
}
