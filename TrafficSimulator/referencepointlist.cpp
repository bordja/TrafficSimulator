#include "referencepointlist.h"

ReferencePointList::ReferencePointList()
{
    MapObject* p1 = new MapObject(pole, 19.8371305682558, 45.2403304784098, 3306801);
    MapObject* p2 = new MapObject(pole,19.835852304712, 45.2400027202778, 3306959);
    MapObject* p3 = new MapObject(pole, 19.8381933428869, 45.2404551891655, 3284215);
    MapObject* p4 = new MapObject(pole, 19.8380630431273, 45.240062964672, 3283042);
    MapObject* p5 = new MapObject(pole, 19.8345589037287, 45.2392289539056, 3284450);
    MapObject* p6 = new MapObject(pole, 19.8348513016058, 45.2393106533107, 3284451);
    MapObject* p7 = new MapObject(pole, 19.8351439792443, 45.2393911669633, 3284452);
    MapObject* p8 = new MapObject(pole, 19.8354366979173, 45.2394728321365, 3284453);
    MapObject* p9 = new MapObject(pole, 19.8357333311793, 45.2395555802512, 3284454);
    MapObject* p10 = new MapObject(pole, 19.8360249884538, 45.2396369091563, 3284455);
    MapObject* p11 = new  MapObject(pole, 19.8363148966923, 45.2397191014189, 3284456);
    MapObject* p12 = new  MapObject(pole, 19.8366070640388, 45.2398011089359, 3284457);
    MapObject* p13 = new  MapObject(pole, 19.8369002197694, 45.2398815329502, 3284458);
    MapObject* p14 = new  MapObject(pole, 19.8371930346256, 45.2399637255231, 3284459);
    MapObject* p15 = new  MapObject(pole, 19.8374866431909, 45.2400457634268, 3284460);
    MapObject* p16 = new  MapObject(pole, 19.8377318114169, 45.2401146922195, 3284461);
    MapObject* p17 = new  MapObject(pole, 19.8381016120946, 45.2402188836176, 3284462);
    MapObject* p18 = new  MapObject(pole, 19.8383297186554, 45.2402825535426, 3284463);
    MapObject* p19 = new  MapObject(pole, 19.8378687935717, 45.2404175182494, 3284216);
    MapObject* p20 = new  MapObject(pole, 19.8376413350343, 45.2403226279311, 3284217);
    MapObject* p21 = new  MapObject(pole, 19.8373802633169, 45.2402358636158, 3284218);
    MapObject* p22 = new  MapObject(pole, 19.8370856323088, 45.240153329113, 3284219);
    MapObject* p23 = new  MapObject(pole, 19.8367990190159, 45.2400649180941, 3284220);
    MapObject* p24 = new  MapObject(pole, 19.8365166678869, 45.2399861694715, 3284221);
    MapObject* p25 = new  MapObject(pole, 19.8362317005224, 45.2399064576629, 3284222);
    MapObject* p26 = new  MapObject(pole, 19.8359449609075, 45.2398268710969, 3284223);
    MapObject* p27 = new  MapObject(pole, 19.8356629730005, 45.2397510396861, 3284224);
    MapObject* p28 = new  MapObject(pole, 19.8352936637815, 45.2396431018536, 3284225);
    MapObject* p29 = new  MapObject(pole, 19.8350034769239, 45.2395640447638, 3284226);
    MapObject* p30 = new  MapObject(pole, 19.8347233084554, 45.2394853139176, 3284227);
    MapObject* p31 = new  MapObject(pole, 19.8344366643589, 45.2394062464638, 3284228);
    MapObject* p32 = new  MapObject(pole, 19.8381560091267, 45.2398922192009, 3283041);

    poles.append(p1);
    poles.append(p2);
    poles.append(p3);
    poles.append(p4);
    poles.append(p5);
    poles.append(p6);
    poles.append(p7);
    poles.append(p8);
    poles.append(p9);
    poles.append(p10);
    poles.append(p11);
    poles.append(p12);
    poles.append(p13);
    poles.append(p14);
    poles.append(p15);
    poles.append(p16);
    poles.append(p17);
    poles.append(p18);
    poles.append(p19);
    poles.append(p20);
    poles.append(p21);
    poles.append(p22);
    poles.append(p23);
    poles.append(p24);
    poles.append(p25);
    poles.append(p26);
    poles.append(p27);
    poles.append(p28);
    poles.append(p29);
    poles.append(p30);
    poles.append(p31);
    poles.append(p32);
}

Point* ReferencePointList::getPoleLocationFromId(int id)
{
    for(int i = 0; i < poles.size(); i++)
    {
        if(poles[i]->getId() == id)
        {
            return (poles[i]->getLocationByAddress());
        }
    }
    return nullptr;
}
