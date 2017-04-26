#include <cstdlib>
#include <cstdio>
#include <math.h>
#include <gsl/gsl_rng.h>
#include <sys/time.h>

#define A0 0.398942270991
#define A1 0.020133760596
#define A2 0.002946756074
#define B1 0.217134277847
#define B2 0.018576112465
#define B3 0.000643163695
#define C0 1.398247031184
#define C1 -0.360040248231
#define C2 0.022719786588
#define D0 1.460954518699
#define D1 -0.305459640162
#define D2 0.038611796258
#define D3 -0.003787400686
#define E0 2.50662823884
#define E1 -18.61500062529
#define E2 41.39119773534
#define E3 -25.44106049637
#define F0 -8.47351093090
#define F1 23.08336743743
#define F2 -21.06224101826
#define F3 3.13082909833
#define G0 0.3374754822726147
#define G1 0.9761690190917186
#define G2 0.1607979714918209
#define G3 0.0276438810333863
#define G4 0.0038405729373609
#define G5 0.0003951896511919
#define G6 0.0000321767881768
#define G7 0.0000002888167364
#define G8 0.0000003960315187

/**
 * Draws a random number in \f$[0,1]\f$ via rand.
 *
 * @return The drawn random number.
 */
double randomNumber01()
{
	return (double) rand() / RAND_MAX;
}

/**
 * Draws a random number in \f$[0,1]\f$ via the gsl.
 *
 * @param r A pointer to the random number generator which is used.
 *
 * @return The drawn random number.
 */
double randomNumber01GSL(gsl_rng* r)
{
	return gsl_rng_uniform(r);
}

/**
 * Draws a random number in \f$[a,b]\f$ via the gsl.
 *
 * @param r A pointer to the random number generator which is used.
 * @param a Left bound of interval.
 * @param b Right bound of interval.
 *
 * @return The drawn random number.
 */
double randomNumberabGSL(gsl_rng* r, double a, double b)
{
    double x = randomNumber01GSL(r);
    x = x*(b-a);
    x = x+a;
    return x;
}

/**
 * Calculates the density value of a standard normal distribution at point x.
 *
 * @param x Parameter for density function.
 *
 * @return The density value.
 */
double standardNormalDensity(double x)
{
    return exp(-pow(x,2)/2.)/sqrt(2.*M_PI);
}

/**
 * Draws a standard normal distributed random number by using the rejection sampling algorithm.
 * 
 * @param r A pointer to the random number generator which is used.
 *
 * @return The drawn random number.
 */
double rejectionSampling(gsl_rng* r)
{
    double x;
    double b = 1/sqrt(2.*M_PI);
    double y;
    do
    {
        x = randomNumberabGSL(r, -6., 6.); // Interval [-2,2] for result in Figure 1
        y = randomNumberabGSL(r, 0, b);
    }while(y>standardNormalDensity(x));
    
	return x;
}

/**
 * Draws a standard normal distributed random number by using the Box-Muller method.
 * 
 * @param r A pointer to the random number generator which is used.
 */
double* boxMuller(gsl_rng* r)
{
    double u[2];
    double *z = new double[2];

    u[0] = randomNumber01GSL(r);
    u[1] = randomNumber01GSL(r);

    z[0] = sqrt(-2.*log(u[0]))*sin(2.*M_PI*u[1]);
    z[1] = sqrt(-2.*log(u[0]))*cos(2.*M_PI*u[1]);

    return z;
}

/**
 * Calculates the cumulative distribution function of the standard normal distribution for a parameter x.
 * 
 * @param x The parameter for the CDF of the standard normal distribution.
 *
 * @return The value of the CDF at x.
 */
double normalCDF(double x)
{
    double x2;
    if(x<0.0)
        return 1.0-normalCDF(-x);
    if(x<=1.87)
    {
        x2 = x*x;
        return 0.5+x*(A0+(A1+A2*x2)*x2)/(1.0+(B1+(B2+B3*x2)*x2)*x2);
    }
    else if(x<6.0)
    {
        return 1.0-pow((C0+(C1+C2*x)*x)/(D0+(D1+(D2+D3*x)*x)*x), 16.0);
    }
    return 1.0;
}

double normalInverseCDF(double x)
{
    double p = x-0.5;
    double r;
    if(fabs(p)<0.42)
    {
        r = pow(p, 2.);
        return p*(((E3*r+E2)*r+E1)*r+E0)/((((F3*r+F2)*r+F1)*r+F0)*r+1.0);
    }
    else
    {
        if(p<0)
            r = x;
        else
            r = 1-x;
        
        r = log(-log(r));
        r = G0+r*(G1+r*(G2+r*(G3+r*(G4+r*(G5+r*(G6+r*(G7+r*G8)))))));

        if(p<0)
            return -r;
        else
            return r;
    }
}

double sigmaAlgorithm(double values[], int N)
{
    double alpha = values[0];
    double beta = 0.;
    int i;
    double sigma = 1.;
    for(i=1; i<N; i++)
    {
        double gamma = values[i]-alpha;
        alpha = alpha+gamma/(i+1);
        beta = beta+gamma*gamma*i/(i+1);
        sigma = sqrt(beta/i);   // An dieser Stelle eigentlich unnötig, hinter der For-Schleife sinnvoller.
    }

    return sigma;
}

double normalFromStandardNormalDistribution(gsl_rng* r, double mu, double sigma)
{
    return mu+sigma*rejectionSampling(r);
}

// enable doxygen processing for this header:
/** @file */

/** 
 * Main function for random number evaluation.
 * A first random number \f$x_1\f$ is drawn via rand. 
 * A second random number \f$x_2\f$ is drawn via gsl_rng_uniform.
 *
 * @param argc The number of arguments provided.
 * @param argv An array of arguments (argv[0] is the name of the
 * executable).
 *
 * @return If everything worked fine, 0 is returned.
 */
int main(int argc, char* argv[]) 
{
	gsl_rng* r;
	r = gsl_rng_alloc(gsl_rng_mt19937);
    struct timeval tv;
    gettimeofday(&tv,0);
    unsigned long seed = tv.tv_sec;
    gsl_rng_set(r, seed);
    //Print random number created by function randomNumber01
	printf("Random Number: %lf\n", randomNumber01());
    //Print random number create by function randomNumber01GSL
	printf("Random Number GSL: %lf\n", randomNumber01GSL(r));

    //Open or create file "values.txt" to insert the values calculated by the rejection sampling algorithm    
    FILE *f = fopen("values.txt", "w");
    //Write 1000000 values into file
    for(int i=0; i<1000000; i++)
        fprintf(f, "%lf\n", rejectionSampling(r));
    //Close file "values.txt"
    fclose(f);

    //Task 4
    printf("Normal CDF at 0.5; %lf\n", normalCDF(0.5));

    //Task 6
    double *z = boxMuller(r);
    printf("Box Muller 1: %lf\n", z[0]);
    printf("Box Muller 2: %lf\n", z[1]);

    //Task 8
    int N = 1000000;
    double values[N];
    for(int i=0; i<N; i++)
        values[i] = rejectionSampling(r);
    
    double mu = 0.;
    for(int i=0; i<N; i++)
        mu += values[i];
    mu = mu/N;
    printf("Mu: %lf\n", mu);

    double sigma = 0.;
    for(int i=0; i<N; i++)
        sigma += pow((values[i]-mu), 2.);
    sigma = sigma/(N-1);
    printf("Sigma naively: %lf\n", sigma);

    printf("Sigma via algorithm: %lf\n", sigmaAlgorithm(values, N));
        
    printf("Mu: 5\n");

    //Task 9
    for(int k=1; k<=10000; k*=10)
    {
        double newValues[k];
        printf("Given Sigma: 0.1\n");
        for(int i=0; i<k; i++)
            newValues[i] = normalFromStandardNormalDistribution(r, 5., 0.1);
        printf("Sigma N=%i: %lf\n", k, sigmaAlgorithm(newValues, k));

        printf("Given Sigma: 2\n");
        for(int i=0; i<k; i++)
            newValues[i] = normalFromStandardNormalDistribution(r, 5., 2);
        printf("Sigma N=%i: %lf\n", k, sigmaAlgorithm(newValues, k));

        printf("Given Sigma: 10\n");
        for(int i=0; i<k; i++)
            newValues[i] = normalFromStandardNormalDistribution(r, 5., 10);
        printf("Sigma N=%i: %lf\n\n", k, sigmaAlgorithm(newValues, k));
    }

    //Free allocated space for gsl
    gsl_rng_free(r);

	return 0;
}
