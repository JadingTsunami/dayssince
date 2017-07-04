/* MIT License
 * 
 * Copyright (c) 2017 JadingTsunami
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
 * Diffs two times, prints the result and exits.
 *
 * Run with no args to diff against last saved time
 * Run with "reset" as the arg to diff against the
 *  current time next time.
 *
 * TODO: Show months, years, etc. as warranted
 * TODO: Functionize this; it's a mess (seriously) :)
 */

double timeFromFile( char* filename );

int main( int argc, char** argv ) {
    if( argc == 1 ) {
        // Operation: diff against saved time
        double diff;
        int days, hours, minutes, seconds;

        diff = timeFromFile( "time.dump" );

        days = (int)(diff/(3600*24));

        diff -= days*3600*24;

        hours = (int)(diff/3600);

        diff -= hours*3600;

        minutes = (int)(diff/60);

        diff -= minutes*60;

        seconds = (int) diff;

        printf("%d days since (%02d:%02d:%02d)\n", days, hours, minutes, seconds );

    } else if( argc == 2 ) {
        // TODO: see if it's a file, then open that?
        // Operation: save current time
        //
        FILE* outfile;
        if( strcmp(argv[1],"reset") == 0  ) {
            outfile = fopen( "time.dump", "w" );
        } else {
            // Operation: diff against saved time
            double diff;
            int days, hours, minutes, seconds;

            diff = timeFromFile( argv[1] );

            days = (int)(diff/(3600*24));

            diff -= days*3600*24;

            hours = (int)(diff/3600);

            diff -= hours*3600;

            minutes = (int)(diff/60);

            diff -= minutes*60;

            seconds = (int) diff;

            printf("%d days since (%02d:%02d:%02d)\n", days, hours, minutes, seconds );


            exit(0);
        }

        if( !outfile ) {
            perror( "Couldn't open time.dump file.\n" );
        } else {
            // get the time and save the struct,
            // line by line
            // TODO Put into function
            struct tm now;
            char buf[64];
            time_t tnow = time(NULL);
            now = *localtime( &tnow );
            sprintf( buf, "%d",  now.tm_sec ) ;
            fputs(buf, outfile);
            fputs(  "\n" , outfile );
            sprintf( buf, "%d",  now.tm_min ) ;
            fputs(buf, outfile);
            fputs(  "\n" , outfile );
            sprintf( buf, "%d",  now.tm_hour ) ;
            fputs(buf, outfile);
            fputs(  "\n" , outfile );
            sprintf( buf, "%d",  now.tm_mday ) ;
            fputs(buf, outfile);
            fputs(  "\n" , outfile );
            sprintf( buf, "%d",  now.tm_mon ) ;
            fputs(buf, outfile);
            fputs(  "\n" , outfile );
            sprintf( buf, "%d",  now.tm_year ) ;
            fputs(buf, outfile);
            fputs(  "\n" , outfile );
            sprintf( buf, "%d",  now.tm_wday ) ;
            fputs(buf, outfile);
            fputs(  "\n" , outfile );
            sprintf( buf, "%d",  now.tm_yday ) ;
            fputs(buf, outfile);
            fputs(  "\n" , outfile );
            sprintf( buf, "%d",  now.tm_isdst ) ;
            fputs(buf, outfile);
            fputs(  "\n" , outfile );
            fflush(outfile);
            fclose(outfile);
            printf("Time saved.\n");
        }
    } else if( argc > 3 && argc < 8) {
        // assume they put the time on the command line
        // MM DD YYYY HH:MM:SS
        //      or YY    (else all 0s)
        double diff;
        time_t target_time;
        int days, hours, minutes, seconds;
        char* sinceortil = "since";

        struct tm given = { 0 };
        given.tm_mon   = atoi( argv[1] )-1;
        given.tm_mday  = atoi( argv[2] );
        given.tm_year  = atoi( argv[3] );
        // FIXME: should perform error checking
        if( argc > 6 ) {
            given.tm_hour  = atoi( argv[4] );
            given.tm_min   = atoi( argv[5] );
            given.tm_sec   = atoi( argv[6] );
        }

        if( given.tm_year > 2000 ) {
            given.tm_year = (given.tm_year-2000)+100;
        } else if( given.tm_year < 100 ) {
            given.tm_year += 100;
        }

        target_time = mktime( &given );

        diff = difftime( time(NULL), target_time );

        if( diff < 0 ) {
            diff = diff*(-1);
            sinceortil = "until";
        }

        days = (int)(diff/(3600*24));

        diff -= days*3600*24;

        hours = (int)(diff/3600);

        diff -= hours*3600;

        minutes = (int)(diff/60);

        diff -= minutes*60;

        seconds = (int) diff;

        printf("%d days %s (%02d:%02d:%02d)\n", days, sinceortil, hours, minutes, seconds );
    } else {
        printf( "Usage:                                      \n" );
        printf( " 3 modes:                                   \n" );
        printf( "    %s                                      \n", argv[0] );
        printf( "      How long since last reset?            \n" );
        printf( "    %s reset                                \n", argv[0] );
        printf( "      Write a file with the current time for\n" );
        printf( "      comparison later on (writes a file    \n" );
        printf( "      called \"time.dump\" in the cwd)      \n" );
        printf( "    %s MM DD YY[YY] [HH MM SS]              \n", argv[0] );
        printf( "      Time from now until/since time given  \n" );
        exit(255);
    }
}


double timeFromFile( char* filename ) {
    FILE* infile = fopen( filename,"r" );

    if(!infile) {
        perror( "Couldn't open your input file. Did you run the \"reset\" operation first to create one?\n" );
        exit(1);
    } else {
        time_t target_time;
        char buf[64];

        struct tm saved = { 0 };
        saved.tm_sec   = atoi( fgets( buf, 64, infile ) );
        saved.tm_min   = atoi( fgets( buf, 64, infile ) );
        saved.tm_hour  = atoi( fgets( buf, 64, infile ) );
        saved.tm_mday  = atoi( fgets( buf, 64, infile ) );
        saved.tm_mon   = atoi( fgets( buf, 64, infile ) );
        saved.tm_year  = atoi( fgets( buf, 64, infile ) );
        saved.tm_wday  = atoi( fgets( buf, 64, infile ) );
        saved.tm_yday  = atoi( fgets( buf, 64, infile ) );
        saved.tm_isdst = atoi( fgets( buf, 64, infile ) );
        target_time = mktime( &saved );

        return difftime( time(NULL), target_time );
    }
    return 0;
}
