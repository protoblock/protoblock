#include "playerloader.h"

decltype(MikeClayLoader::headers) MikeClayLoader::headers{
{"statsid",0},
{"pos",1},
{"name",2},
{"team",3},
{"pyds",7},
{"ptd",8},
{"pint",9},
{"ruyds",12},
{"rutd",13},
{"rec",15},
{"reyds",16},
{"retd",17},
{"fuml",18},
{"sack",29},
{"sfty",30},
{"int",31},
{"fumble",33},
{"dtd",34},
{"pa0",35},
{"pa6",36},
{"pa13",37},
{"pa20",38},
{"fg19",42},
{"fg29",43},
{"fg39",44},
{"fg49",45},
{"fg50p",46},
{"xp",52},
};


decltype(MikeClayLoader::import) MikeClayLoader::import{

/*
    {"28800", 1609},
    {"28697", 1127},
    {"26853", 1010},


    /*
    {"27359", 1365},
    {"26834", 1736},
    {"27305", 1699},
    {"25290", 1765},
    {"29070", 1652},
*/
};

decltype (PlayerLoaderTR::byes) PlayerLoaderTR::byes {
    { 1 , {} },
    { 2 , {} },
    { 3 , {} },
    { 4 , {"NE","TEN"} },
    { 5 , {"CAR", "MIA", "MIN", "NYJ"} },
    { 6 , {"DAL", "OAK", "STL", "TB"} },
    { 7 , {"CHI", "CIN", "DEN", "GB"} },
    { 8 , {"BUF", "JAC", "PHI", "WAS"} },
    { 9 , {"ARI", "BAL", "DET", "HOU", "KC", "SEA"} },
    { 10 , {"ATL", "IND", "SD", "SF"} },
    { 11 , {"CLE", "NO", "NYG", "PIT"} },
    { 12 , {} },
    { 13 , {} },
    { 14 , {} },
    { 15 , {} },
    { 16 , {} },
    { 17 , {} },
};
