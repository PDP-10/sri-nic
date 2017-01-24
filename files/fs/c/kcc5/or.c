/* N3EMO Orbit Simulator routines */

/* Copyright (C) 1986 Robert W. Berger
   May be used for non-commercial purposes without prior written permission. */

#include <stdio.h>
#include <math.h>

#define PI 3.14159265
#define PI2 (PI*2)
#define MinutesPerDay (24*60.0)
#define SecondsPerDay (60*MinutesPerDay)
#define HalfSecond (0.5/SecondsPerDay)
#define EarthRadius 6378.16             /* Kilometers */
#define EarthFlat (1/298.25)            /* Earth Flattening Coeff. */
#define SiderealSolar 1.0027379093
#define DegreesPerRadian (180/PI)
#define RadiansPerDegree (PI/180)
#define ABS(x) ((x) < 0 ? (-(x)) : (x))
#define SQR(x) ((x)*(x))

#define RefYear 79
#define RefSidereal 0.27518504
#define RefDay 0                /* Day of week */

char *MonthNames[] = { "Jan","Feb","Mar","Apr","May","Jun","Jul",
                        "Aug","Sep","Oct","Nov","Dec" };

int MonthDays[] = {31,28,31,30,31,30,31,31,30,31,30,31};

char *DayNames[] = { "Sunday","Monday","Tuesday","Wednesday","Thursday",
                        "Friday","Saturday"};

/* Solve Kepler's equation                                      */
/* Inputs:                                                      */
/*      MeanAnomaly     Time since last perigee, in radians.    */
/*                      PI2 = one complete orbit.               */
/*      Eccentricity    Eccentricity of orbit's ellipse.        */
/* Output:                                                      */
/*      TrueAnomaly     Angle between perigee, geocenter, and   */
/*                      current position.                       */

double Kepler(MeanAnomaly,Eccentricity)
register double MeanAnomaly,Eccentricity;

{
register double E;              /* Eccentric Anomaly                    */
register double Error;
register double TrueAnomaly;

    E = MeanAnomaly;    /* Initial guess */
    do
        {
        Error = (E - Eccentricity*sin(E) - MeanAnomaly)
                / (1 - Eccentricity*cos(E));
        E -= Error;
        }
   while (ABS(Error) >= 0.000001);

    if (ABS(E-PI) < 0.000001)
        TrueAnomaly = PI;
      else
        TrueAnomaly = 2*atan(sqrt((1+Eccentricity)/(1-Eccentricity))
                                *tan(E/2));
    if (TrueAnomaly < 0)
        TrueAnomaly += PI2;

    return TrueAnomaly;
}

GetSubSatPoint(EpochTime,EpochRAAN,EpochArgPerigee,Inclination,
        Eccentricity,RAANPrecession,PerigeePrecession,Time,TrueAnomaly,
                Latitude,Longitude)

double EpochTime,EpochRAAN,EpochArgPerigee,Inclination,Eccentricity;
double RAANPrecession,PerigeePrecession,Time;
double TrueAnomaly,*Longitude,*Latitude;
{
    double RAAN,ArgPerigee;
    int i;

    ArgPerigee = EpochArgPerigee + (Time-EpochTime)*PerigeePrecession;

    *Latitude = asin(sin(Inclination)*sin(ArgPerigee+TrueAnomaly));

    RAAN = EpochRAAN - (Time-EpochTime)*RAANPrecession;

    *Longitude = -acos(cos(TrueAnomaly+ArgPerigee)/cos(*Latitude));
    if ((Inclination > PI/2 && *Latitude < 0)
          || (Inclination < PI/2 && *Latitude > 0))
        *Longitude = -*Longitude;
    *Longitude += RAAN;

    /* Convert from celestial to terrestrial coordinates */
    *Longitude -= PI2*(Time*SiderealSolar + RefSidereal);

    /* Make West be positive */
    *Longitude = -*Longitude;

    /* i = floor(Longitude/2*pi)        */
    i = *Longitude/PI2;
    if(i < 0)
        i--;

    *Longitude -= i*PI2;
}


GetPrecession(SemiMajorAxis,Eccentricity,Inclination,
        RAANPrecession,PerigeePrecession)
double SemiMajorAxis,Eccentricity,Inclination;
double *RAANPrecession,*PerigeePrecession;
{
  *RAANPrecession = 9.95*pow(EarthRadius/SemiMajorAxis,3.5) * cos(Inclination)
                 / SQR(1-SQR(Eccentricity)) * RadiansPerDegree;

  *PerigeePrecession = 4.97*pow(EarthRadius/SemiMajorAxis,3.5)
         * (5*SQR(cos(Inclination))-1)
                 / SQR(1-SQR(Eccentricity)) * RadiansPerDegree;
}

GetBearings(SiteLat,SiteLong,SiteAltitude,
        SatLat,SatLong,Radius,Azimuth,Elevation,Range)
double SiteLat,SiteLong,SiteAltitude,SatLat,SatLong,Radius;
double *Azimuth,*Elevation,*Range;
{
    double SinSiteLat,sinSiteLong,SinSatLat,SinSatLong;
    double CosSiteLat,cosSiteLong,CosSatLat,CosSatLong;
    double CosBeta,SinBeta;
    double LongDiff;

    SinSiteLat = sin(SiteLat); sinSiteLong = sin(SiteLong);
    CosSiteLat = cos(SiteLat); cosSiteLong = cos(SiteLong);
    SinSatLat = sin(SatLat); SinSatLong = sin(SatLong);
    CosSatLat = cos(SatLat); CosSatLong = cos(SatLong);

    LongDiff = SiteLong - SatLong;
    CosBeta = SinSiteLat*SinSatLat+CosSiteLat*CosSatLat*cos(LongDiff);
    SinBeta = sqrt(1-SQR(CosBeta));

    *Azimuth = acos((SinSatLat- SinSiteLat*CosBeta)/CosSiteLat/SinBeta);

    if (LongDiff < -PI)
        LongDiff += PI2;
    if (LongDiff > PI)
        LongDiff -= PI2;

    if (LongDiff < 0)
        *Azimuth = PI2 - *Azimuth;

    /* Convert to geocentric radius */
    SiteAltitude += EarthRadius*(1-EarthFlat/2+EarthFlat/2*cos(2*SiteLat));

    *Elevation = atan((Radius*CosBeta-(SiteAltitude))
                        /(Radius*SinBeta));

    *Range = sqrt(SQR(Radius) + SQR(SiteAltitude)
                    -2*Radius*SiteAltitude*CosBeta);
}


SPDATE(Str,Day)
char *Str;
{
    int Month,Year,DayOfWeek;

    DayOfWeek = (Day-RefDay) % 7;

    Year = RefYear;

    while (Day <= 0)
        {
        Year -= 1;
        if (Year%4 == 0)
            Day += 366;
         else
            Day += 365;
        }

    while ((Year%4 == 0 && Day > 366) || (Year%4 != 0 && Day > 365))
        {
        if (Year%4 == 0)
            Day -= 366;
         else
            Day -= 365;
        Year += 1;
        }

    if (Year%4 == 0)
        MonthDays[1] += 1;      /* Leap year */

    Month = 0;
    while (Day > MonthDays[Month])
        {
        Day -= MonthDays[Month];
        Month += 1;
        }

    sprintf(Str,"%s  %d %s %d",DayNames[DayOfWeek],Day,
                MonthNames[Month],Year);

    if (Year%4 == 0)
        MonthDays[1] -= 1;      /* Leap year */
}


SPTIME(Str,Time)
char *Str;
double Time;
{
    int day,hours,minutes,seconds;

    day = Time;
    Time -= day;
    if (Time < 0)
        Time += 1.0;   /* Correct for truncation problems with negatives */

    hours = Time*24;
    Time -=  hours/24.0;

    minutes = Time*MinutesPerDay;
    Time -= minutes/MinutesPerDay;

    seconds = Time*SecondsPerDay;
    seconds -= seconds/SecondsPerDay;

    sprintf(Str,"%02d%02d:%02d",hours,minutes,seconds);
}

PrintDate(OutFile,Day)
FILE *OutFile;
{
    char str[100];

    SPDATE(str,Day);
    fprintf(OutFile,"%s",str);
}

PrintTime(OutFile,Time)
FILE *OutFile;
double Time;
{
    char str[100];

    SPTIME(str,Time);
    fprintf(OutFile,"%s",str);
}


/* Get the Day Number for a given date. January 1 of the reference year
   is day 1. Note that the Day Number may be negative, if the sidereal
   reference is in the future.                                          */

double GetDay(Year,Month,Day)
double Day;
{
    int TmpYear;

    TmpYear = Year;

    while (TmpYear > RefYear)
        {
        TmpYear -= 1;
        if (TmpYear%4 == 0)
            Day += 366;
         else
            Day += 365;
        }

    while (TmpYear < RefYear)
        {
        if (TmpYear%4 == 0)
            Day -= 366;
         else
            Day -= 365;
        TmpYear += 1;
        }

    if (Year%4 == 0)
        MonthDays[1] += 1;      /* Leap year */

     while (Month > 1)
        {
        Month -= 1;
        Day += MonthDays[Month-1];
        }

    if (Year%4 == 0)
        MonthDays[1] -= 1;      /* Leap year */

    return Day;
}
