/* Copyright (C) 1986,1987 Robert W. Berger
   May be used for non-commercial purposes without prior written permission. */

/* Change Log
	3/24/87		v2.3 Adapted for new kepler.dat format.
			     Allow beacon frequencies in mode.dat.
			     Use decay rate for drag compensation.

	5/10/86		v2.2 Added single character aliases for satellite
			names.

	4/30/86		v2.1 Print blank line if satellite dips below
			horizon and reappears during same orbit and day

	4/29/86		v2.0 Changed GetSatelliteParams() to use AMSAT's
			"kepler.dat" file. Moved schedule to "mode.dat" file.

        4/22/86         v1.3  Inserted N8FJB's suggestions for variable naming
                        which maintain 8 character uniqueness.
                        Also removed "include" file orbitr.h, which had two
                        definitions of external functions defined in orbit.c
			    -K3MC

        4/1/86          v1.2  Corrected a scanf conversion to %d for an int
                        type.    -K3MC

        3/19/86         v1.1  Changed GetSatelliteParams to not pass NULL
                        to sscanf.

                                                                        */

#define DRAG 1

#include <stdio.h>
#include <math.h>

extern double Kepler();
extern double GetDay();


#define PI 3.14159265
#define PI2 (PI*2)
#define MinutesPerDay (24*60.0)
#define SecondsPerDay (60*MinutesPerDay)
#define HalfSecond (0.5/SecondsPerDay)
#define EarthRadius 6378.16             /* Kilometers           */
#define C 2.997925e5                    /* Kilometers/Second    */
#define DegreesPerRadian (180/PI)
#define RadiansPerDegree (PI/180)
#define ABS(x) ((x) < 0 ? (-(x)) : (x))
#define SQR(x) ((x)*(x))

#define MaxModes 10
typedef struct {
                int MinPhase,MaxPhase;
                char ModeStr[20];
               }  ModeRec;

char VersionStr[] = "N3EMO Orbit Simulator  v2.3";

    /*  Keplerian Elements and misc. data for the satellite              */
    double  EpochDay;                   /* time of epoch                 */
    double EMeanAnomaly;            /* Mean Anomaly at epoch         */
    long EpochOrbitNum;                 /* Integer orbit # of epoch      */
    double EpochRAAN;                   /* RAAN at epoch                 */
    double EMeanMotion;             /* Revolutions/day               */
    double OrbitalDecay;                /* Revolutions/day^2             */
    double EpochArgPerigee;             /* argument of perigee at epoch  */
    double Eccentricity;
    double Inclination;
    char SatName[100];
    int ElementSet;
    double BeaconFreq;                  /* Mhz, used for doppler calc    */
    double MaxPhase;                    /* Phase units in 1 orbit        */
    double perigeePhase;
    int NumModes;
    ModeRec Modes[MaxModes];
    int PrintApogee;
    int TimeZone;			/* timezone correction in hours    */
    char TimeSZoneString[40];		/* PST is -8			   */

    /* Simulation Parameters */

    double StartTime,EndTime, StepTime; /* In Days, 1 = New Year        */
                                        /*      of reference year       */

    /* Site Parameters */
    char SiteName[100];
    double SiteLat,SiteLong,SiteAltitude,SiteMinElev;


/* List the satellites in kepler.dat, and return the number found */
ListSatellites()
{
    char str[100];
    FILE *InFile;
    char satchar;
    int NumSatellites;

    printf("Available satellites:\n");

    if ((InFile = fopen("kepler.dat","r")) == 0)
        {
	printf("\"kepler.dat\" not found\n");
	exit(-1);
	}

    satchar = 'a';
    NumSatellites = 0;
    while (fgets(str,100,InFile))
	if (sscanf(str,"Satellite: %s",str) == 1)
	    {
	    printf("	%c) %s\n",satchar++,str);
	    NumSatellites++;
	    }

    fclose(InFile);

    return NumSatellites;
}

/* Match and skip over a string in the input file. Exits on failure. */

MatchStr(InFile,FileName,Target)
FILE *InFile;
char *FileName,*Target;
{
    char str[100];

    fgets(str,strlen(Target)+1,InFile);
    if (strcmp(Target,str))
       {
       printf("%s: found \"%s\" while expecting \"%s\n\"",FileName,str,Target);
       exit(-1);
       }
}

GetSatelliteParams()
{
    FILE *InFile;
    char str[100];
    int EpochYear;
    double EpochHour,EMinute,EpochSecond;
    int found;
    int NumSatellites;
    char satchar;

    NumSatellites = ListSatellites();

    found = 0;

    while (!found)
	{
	printf("Letter or satellite name :");
	gets(SatName);

	if ((InFile = fopen("kepler.dat","r")) == 0)
	    {
	    printf("kepler.dat not found\n");
	    exit(-1);
	    }

	if (strlen(SatName) == 1)
	    {			/* use single character label */
	    satchar = SatName[0];
	    if (satchar < 'a' || satchar > 'a'+NumSatellites-1)
	        {
	    	printf("'%c' is out of range\n",satchar);
		fclose(InFile);
		continue;
		}

	    do
		do  /* find line beginning with "Satellite: " */
		    fgets(str,100,InFile);
		while (sscanf(str,"Satellite: %s",SatName) != 1);
	     while (satchar-- > 'a');
	    found = 1;
	    }

	 else
	     {
	     while (!found)  /* use satellite name */
            	{
	    	if (! fgets(str,100,InFile))
	    	    break;	/* EOF */

	    	if (sscanf(str,"Satellite: %s",str) == 1
		    && strcmp(SatName,str) == 0)
			found = 1;
	        }

	    if (!found)
		{
		printf("Satellite %s not found\n",SatName);
		fclose(InFile);
		}
	    }
	}

    BeaconFreq = 146.0;  /* Default value */

    fgets(str,100,InFile);	/* Skip line */

    MatchStr(InFile,"kepler.dat","Epoch time:");
    fgets(str,100,InFile);
    sscanf(str,"%lf",&EpochDay);

    EpochYear = EpochDay / 1000.0;
    EpochDay -= EpochYear*1000.0;
    EpochDay = GetDay(EpochYear,1,EpochDay);
    fgets(str,100,InFile);

    if (sscanf(str,"Element set: %ld",&ElementSet) == 0)
       {   /* Old style kepler.dat */
       MatchStr(InFile,"kepler.dat","Element set:");
       fgets(str,100,InFile);
       sscanf(str,"%d",&ElementSet);
       }

    MatchStr(InFile,"kepler.dat","Inclination:");
    fgets(str,100,InFile);
    sscanf(str,"%lf",&Inclination);
    Inclination *= RadiansPerDegree;

    MatchStr(InFile,"kepler.dat","RA of node:");
    fgets(str,100,InFile);
    sscanf(str,"%lf",&EpochRAAN);
    EpochRAAN *= RadiansPerDegree;

    MatchStr(InFile,"kepler.dat","Eccentricity:");
    fgets(str,100,InFile);
    sscanf(str,"%lf",&Eccentricity);

    MatchStr(InFile,"kepler.dat","Arg of perigee:");
    fgets(str,100,InFile);
    sscanf(str,"%lf",&EpochArgPerigee);
    EpochArgPerigee *= RadiansPerDegree;

    MatchStr(InFile,"kepler.dat","Mean anomaly:");
    fgets(str,100,InFile);
    sscanf(str,"%lf",&EMeanAnomaly);
    EMeanAnomaly *= RadiansPerDegree;

    MatchStr(InFile,"kepler.dat","Mean motion:");
    fgets(str,100,InFile);
    sscanf(str,"%lf",&EMeanMotion);

    MatchStr(InFile,"kepler.dat","Decay rate:");
    fgets(str,100,InFile);
    sscanf(str,"%lf",&OrbitalDecay);

    MatchStr(InFile,"kepler.dat","Epoch rev:");
    fgets(str,100,InFile);
    sscanf(str,"%ld",&EpochOrbitNum);

	while (1)
	    {
	    if (! fgets(str,100,InFile))
		break;	/* EOF */
	    if (strlen(str) <= 2)
	    	break;  /* Blank line */
	    sscanf(str,"Beacon: %lf",&BeaconFreq);
	    }

    PrintApogee = (Eccentricity >= 0.3);

    perigeePhase = 0; MaxPhase = 360; /* Default values */
    NumModes = 0;

    if ((InFile = fopen("mode.dat","r")) == 0)
	return;

    found = 0;
    while (!found)
        {
	if (! fgets(str,100,InFile))
	    break;	/* EOF */
	if (sscanf(str,"Satellite: %s",str) == 1
	    && strcmp(SatName,str) == 0)
		found = 1;
	}

    if (found)
	{
	while (1)
	    {
	    if (! fgets(str,100,InFile))
		break;	/* EOF */
	    if (strlen(str) <= 2)
	    	break;  /* Blank line */
	    sscanf(str,"Beacon: %lf",&BeaconFreq);
	    sscanf(str,"Perigee phase: %lf",&perigeePhase);
	    sscanf(str,"Max phase: %lf",&MaxPhase);

	    if (sscanf(str,"Mode: %20s from %d to %d",Modes[NumModes].ModeStr,
	    &Modes[NumModes].MinPhase,&Modes[NumModes].MaxPhase) == 3
	      && NumModes < MaxModes)
		  NumModes++;
	    }
	fclose(InFile);
	}



}


GetSiteParams()
{
    FILE *InFile;
    char str[100];

    printf("Site name :");
    gets(str);
    strcat(str,".sit");

    if ((InFile = fopen(str,"r")) == 0)
        {
        printf("%s not found\n",str);
        exit(-1);
        }

    fgets(SiteName,100,InFile);

    fgets(str,100,InFile);
    sscanf(str,"%lf",&SiteLat);
    SiteLat *= RadiansPerDegree;

    fgets(str,100,InFile);
    sscanf(str,"%lf",&SiteLong);
    SiteLong *= RadiansPerDegree;

    fgets(str,100,InFile);
    sscanf(str,"%lf",&SiteAltitude);
    SiteAltitude /= 1000;   /* convert to km */

    fgets(str,100,InFile);
    sscanf(str,"%lf",&SiteMinElev);
    SiteMinElev *= RadiansPerDegree;

    if (fgets(str,100,InFile) != NULL)
	{
	sscanf(str,"%d %s",&TimeZone,TimeSZoneString);
	}
	else
	{
	TimeZone = 0;
	strcpy(TimeSZoneString,"U.T.C.");
	}
}

GetSimulationParams()
{
    double hour,duration;
    int Month,Day,Year;

    printf("Start date (UTC) (Month Day Year) :");
    scanf("%d%d%d",&Month,&Day,&Year);

    StartTime = GetDay(Year,Month,(double) Day);

    printf("Starting Hour (UTC) :");
    scanf("%lf",&hour);
    StartTime += hour/24;

    printf("Duration (Days) :");
    scanf("%lf",&duration);
    EndTime = StartTime + duration;

    printf("Time Step (Minutes) :");
    scanf("%lf",&StepTime);
    StepTime /= MinutesPerDay;
}

PrintMode(OutFile,Phase)
FILE *OutFile;
{
    int CurMode;

    for (CurMode = 0; CurMode < NumModes; CurMode++)
        if ((Phase >= Modes[CurMode].MinPhase
                && Phase <= Modes[CurMode].MaxPhase)
              || ((Modes[CurMode].MinPhase > Modes[CurMode].MaxPhase)
                  && (Phase >= Modes[CurMode].MinPhase
                        || Phase <= Modes[CurMode].MaxPhase)))
            {
            fprintf(OutFile,"    %s",Modes[CurMode].ModeStr);
            break;
            }
}


main()
{
    double ReferenceOrbit;      /* Floating point orbit # at epoch */
    double CurrentTime;         /* In Days                         */
    double CurrentOrbit;
    double AverageMotion,       /* Corrected for drag              */
        CurrentMotion;
    double MeanAnomaly,TrueAnomaly;
    double SemiMajorAxis;
    double Radius;              /* From geocenter                  */
    double RAANPrecession,PerigeePrecession;
    double SSPLat,SSPLong;
    long OrbitNum,PrevOrbitNum;
    int Day,PrevDay;
    double Azimuth,Elevation,Range;
    double PrevRange,Velocity,Doppler;
    int Phase;
    char FileName[100];
    FILE *OutFile;
    int DidApogee;
    double TmpTime,PrevTime;
    int PrevVisible;

    printf("%s\n",VersionStr);

    GetSatelliteParams();
    GetSiteParams();
    GetSimulationParams();

    printf("Output file (RETURN for TTY) :");
    gets(FileName);     /* Skip previous RETURN */
    gets(FileName);


    if (strlen(FileName) > 0)
        {
        if ((OutFile = fopen(FileName,"w")) == 0)
            {
            printf("Can't write to %s\n",FileName);
            exit(-1);
            }
        }
      else OutFile = stdout;

    fprintf(OutFile,"%s Element Set %d\n",SatName,ElementSet);

    fprintf(OutFile,"%s\n",SiteName);

    fprintf(OutFile,"Doppler calculated for freq = %lf MHz\n",BeaconFreq);

    SemiMajorAxis = 331.25 * exp(2*log(MinutesPerDay/EMeanMotion)/3);
    GetPrecession(SemiMajorAxis,Eccentricity,Inclination,&RAANPrecession,
                        &PerigeePrecession);

    ReferenceOrbit = EMeanAnomaly/PI2 + EpochOrbitNum;

    PrevDay = -10000; PrevOrbitNum = -10000;
    PrevTime = StartTime-2*StepTime;

    BeaconFreq *= 1E6;          /* Convert to Hz */

    DidApogee = 0;

    /* Start the loop one step early, to init OldRange */
    for (CurrentTime = StartTime-StepTime; CurrentTime <= EndTime;
                CurrentTime += StepTime)
        {

#if DRAG
        AverageMotion = EMeanMotion
	   + (CurrentTime-EpochDay)*OrbitalDecay/2;
        CurrentMotion = EMeanMotion
	   + (CurrentTime-EpochDay)*OrbitalDecay;
#else
        AverageMotion = EMeanMotion;
        CurrentMotion = EMeanMotion;
#endif
        SemiMajorAxis = 331.25 * exp(2*log(MinutesPerDay/CurrentMotion)/3);

        CurrentOrbit = ReferenceOrbit +
                        (CurrentTime-EpochDay)*AverageMotion;
        OrbitNum = CurrentOrbit;

        MeanAnomaly = (CurrentOrbit-OrbitNum)*PI2;

        TmpTime = CurrentTime;
        if (MeanAnomaly < PI)
            DidApogee = 0;
        if (PrintApogee && !DidApogee && MeanAnomaly > PI)
            {                   /* Calculate Apogee */
            TmpTime -= StepTime;   /* So we pick up later where we left off */
            MeanAnomaly = PI;
            CurrentTime=EpochDay+(OrbitNum-ReferenceOrbit+0.5)/AverageMotion;
            }

        TrueAnomaly = Kepler(MeanAnomaly,Eccentricity);
        Radius = SemiMajorAxis*(1-SQR(Eccentricity))
                        / (1+Eccentricity*cos(TrueAnomaly));

        GetSubSatPoint(EpochDay,EpochRAAN,EpochArgPerigee,
           Inclination,Eccentricity,RAANPrecession,PerigeePrecession,
           CurrentTime,TrueAnomaly,&SSPLat,&SSPLong);
        GetBearings(SiteLat,SiteLong,SiteAltitude,SSPLat,SSPLong,Radius,
            &Azimuth,&Elevation,&Range);

        Velocity = (Range-PrevRange)/((CurrentTime-PrevTime)*SecondsPerDay);
        PrevRange = Range;

        if (Elevation >= SiteMinElev && CurrentTime >= StartTime)
            {
/*            Day = CurrentTime + HalfSecond;	*/
            Day = CurrentTime + HalfSecond + (double)(TimeZone/24.0);
            if (((double) Day) > CurrentTime+HalfSecond)
                Day -= 1;    /* Correct for truncation of negative values */

	    if (OrbitNum == PrevOrbitNum && Day == PrevDay && !PrevVisible)
	    	fprintf(OutFile,"\n");	/* Dipped out of sight; print blank */

            if (OrbitNum != PrevOrbitNum || Day != PrevDay)
                {                       /* Print Header */
                PrintDate(OutFile,Day);
                fprintf(OutFile,"  ----Orbit # %ld-----\n",OrbitNum);
/*                fprintf(OutFile," U.T.C.   Az   El  Doppler  Range");	*/
                fprintf(OutFile," %6.6s   Az   El  Doppler  Range",
				TimeSZoneString);	/* include zone */
                fprintf(OutFile,"  Height  Lat  Long  Phase(%3.0lf)\n",
                                MaxPhase);
                }
            PrevOrbitNum = OrbitNum; PrevDay = Day;
/*            PrintTime(OutFile,CurrentTime + HalfSecond);	*/
            PrintTime(OutFile,CurrentTime + HalfSecond +
			(double) (TimeZone/24.0));	/* include timezone */

            Doppler = -BeaconFreq*Velocity/C;

            fprintf(OutFile,"  %3.0lf  %3.0lf  %6.0lf  %6.0lf",
	        Azimuth*DegreesPerRadian,
                Elevation*DegreesPerRadian,Doppler,Range);

            fprintf(OutFile,"  %6.0lf  %3.0lf  %4.0lf",
                Radius - EarthRadius,SSPLat*DegreesPerRadian,
                SSPLong*DegreesPerRadian);

            Phase = (MeanAnomaly/PI2*MaxPhase + perigeePhase);
            while (Phase < 0)
                Phase += MaxPhase;
            while (Phase >= MaxPhase)
                Phase -= MaxPhase;

            fprintf(OutFile,"  %4d", Phase);
            PrintMode(OutFile,Phase);
            if (PrintApogee && (MeanAnomaly == PI))
                fprintf(OutFile,"    Apogee");
            fprintf(OutFile,"\n");
	    PrevVisible = 1;
            }
	 else
	    PrevVisible = 0;
        if (PrintApogee && (MeanAnomaly == PI))
            DidApogee = 1;
        PrevTime = CurrentTime;
        CurrentTime = TmpTime;
        }
    fclose(OutFile);

}
