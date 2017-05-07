//
//  main.cpp
//  test
//
//  Created by WhoAmI on 28.04.17.
//  Copyright © 2017 Andrei. All rights reserved.
//

#include "random_functions.hpp"

int main(int argc, char* argv[]){
    
    gsl_rng* r;
    
    // seeding
    unsigned long seed = std::time(NULL);
    //memory allocation
    r = gsl_rng_alloc(gsl_rng_mt19937);
    gsl_rng_set(r, seed);

    ///////////////////////////////////////////
    ///////////////////Task_1//////////////////
    ///////////////////////////////////////////
    
    printf( "%lf\n", random_number01());
    
    printf( "%lf\n", random_number_01_GSL(r));
    
    ///////////////////////////////////////////
    ///////////////////Task_2//////////////////
    ///////////////////////////////////////////
    
    int number_samples = 1000000;
    
    std::ofstream myfile;
    
    //opens a file and checks if it was successfully
    //when file will be opened previous content will be deleted
    
    myfile.open("rejection_sampl.txt",std::ios::trunc);
    if (!myfile.is_open()) {
        std::cout<<"Error opening the file"<<std::endl;
    }
    
    //for iteration
    int ittr = 1;
    
    while (ittr<=number_samples) {
        myfile<<rejection_sampl_algo(r)<<std::endl;
        ittr++;
    }
    //closes file
    myfile.close();
    
    
    
    ///////////////////////////////////////////
    ///////////////////Task_3_matlab///////////
    ///////////////////////////////////////////
    
    ///////////////////////////////////////////
    ///////////////////Task_4_5////////////////
    ///////////////////////////////////////////
    
    /**
     * Beasley_Springer_Moro Algorithm
     * it computes inverse c.d.f. for standard normal distribution 
     * at x in order to become a standard normal distributed value
     *
     */
    normal_inverse_cdf(gsl_ran_flat(r, 0, 1));
    
    ///////////////////////////////////////////
    ///////////////////Task_6_7////////////////
    ///////////////////////////////////////////
    
    double mu = 0;
    double sigma = 1;
    
    //writes 1000 samples for 2D plot in the file
    int num_sampl = 1000;
    srand((unsigned)std::time(NULL));
    
    //opens a file and checks if it was successfully
    //when file will be opened previous content will be deleted
    myfile.open("mueller_box.txt",std::ios::trunc);
    if (!myfile.is_open()) {
        std::cout<<"Error opening the file"<<std::endl;
    }
    
    // output of the mueller_box_algo in order to write it to the file
    std::vector<double>* output;
    for (int i = 0; i<num_sampl; i++) {
        
        output = mueller_box_algo(mu, sigma);
        myfile<<(*output)[0]<<" "<<(*output)[1]<<std::endl;
    }
        
    //closes file
    myfile.close();

    ///////////////////////////////////////////
    ///////////////////Task_8_9////////////////
    ///////////////////////////////////////////
    
    
    int N_max = 10000000;
    
    //Three different values for sigma a
    int num_of_sigmas = 3;
    double sigma_s[] = {0.1,1,10};
    double mean = 0;
    
    std::string files[] = {std::to_string((int)sigma_s[0]),std::to_string((int)sigma_s[1]),std::to_string((int)sigma_s[2])};
    std::vector<double> samples;
    
    double sigma_approx = 0.0;
    std::vector<std::vector<double> > sigma_err(num_of_sigmas);
    
    srand((unsigned)std::time(NULL));
    
    std::vector<double>* output_2;
    
    for (int j = 0; j<num_of_sigmas; j++) {
        
        //opens a file and checks if it was successfully
        //when file will be opened previous content will be deleted
        myfile.open("sigma_err_"+files[j]+".txt",std::ios::trunc);
        if (!myfile.is_open()) {
            std::cout<<"Error opening the file"<<std::endl;
        }
        
        for (int N = 10,k = 0; N<=N_max; N = 10*N, k++) {
            
            for (int i = 0; i<N; i++) {
                
                output_2 = mueller_box_algo(mean, sigma_s[j]);
                samples.push_back((*output_2)[0]);
                
            }
            
            sigma_approx = sigma_algorithm(&samples, N);
            sigma_err[j].push_back(fabs(sigma_approx - sigma_s[j]));
            myfile<<sigma_err[j][k]<<" "<<N<<std::endl;
            samples.clear();
        }
        
        //closes file
        myfile.close();
        
    }
    
    
    ///////////////////////////////////////////
    ///////////////////Task_10/////////////////
    ///////////////////////////////////////////
    
    double T_w = 2;
    double delta_t[] = {0.5, 0.01};
    double mu_w = 0.1;
    double sigma_w = 0.2;
    double s0 = 10;
    
    std::ofstream wiener_file;
    std::ofstream asset_file;
    
    
    
    std::vector<double>* w_1 = wiener_process( r, T_w, delta_t[0]);
    std::vector<double>* w_2 = wiener_process( r, T_w, delta_t[0]);
    std::vector<double>* w_3 = wiener_process( r, T_w, delta_t[0]);
    
    wiener_file.open("wiener_process_points_0.5.txt",std::ios::trunc);
    if (!wiener_file.is_open()) {
        std::cout<<"Error opening the file"<<std::endl;
    }
    int M = (int)(T_w/delta_t[0]);
    for(int i=0; i<=M; i++)
        wiener_file<<i*delta_t[0]<<" "<<(*w_1)[i]<<" "<<(*w_2)[i]<<" "<<(*w_3)[i]<<std::endl;
    
    wiener_file.close();
    
    M = (int)(T_w/delta_t[1]);
    std::vector<double>* w_4 = wiener_process( r, T_w, delta_t[1]);
    std::vector<double>* w_5 = wiener_process( r, T_w, delta_t[1]);
    std::vector<double>* w_6 = wiener_process( r, T_w, delta_t[1]);
    
    wiener_file.open("wiener_process_points_0.01.txt",std::ios::trunc);
    if (!wiener_file.is_open()) {
        std::cout<<"Error opening the file"<<std::endl;
    }
    for(int i=0; i<=M; i++)
        wiener_file<<i*delta_t[1]<<" "<<(*w_4)[i]<<" "<<(*w_5)[i]<<" "<<(*w_6)[i]<<std::endl;
    
    wiener_file.close();
    
    
    asset_file.open("correspond_asset_prices_points_0.5.txt",std::ios::trunc);
    if (!asset_file.is_open()) {
        std::cout<<"Error opening the file"<<std::endl;
    }
    
    M = (int)(T_w/delta_t[0]);
    std::vector<double>* s_1 = brownian_motion( r, T_w, delta_t[0], w_1, s0, mu_w, sigma_w);
    std::vector<double>* s_2 = brownian_motion( r, T_w, delta_t[0], w_2, s0, mu_w, sigma_w);
    std::vector<double>* s_3 = brownian_motion( r, T_w, delta_t[0], w_3, s0, mu_w, sigma_w);
    
    for(int i=0; i<=M; i++)
        asset_file<<i*delta_t[0]<<" "<<(*s_1)[i]<<" "<<(*s_2)[i]<<" "<<(*s_3)[i]<<std::endl;
        
    asset_file.close();
    
    asset_file.open("correspond_asset_prices_points_0.01.txt",std::ios::trunc);
    if (!asset_file.is_open()) {
        std::cout<<"Error opening the file"<<std::endl;
    }

    
    M = (int)(T_w/delta_t[1]);
    std::vector<double>* s_4 = brownian_motion( r, T_w, delta_t[1], w_4, s0, mu_w, sigma_w);
    std::vector<double>* s_5 = brownian_motion( r, T_w, delta_t[1], w_5, s0, mu_w, sigma_w);
    std::vector<double>* s_6 = brownian_motion( r, T_w, delta_t[1], w_6, s0, mu_w, sigma_w);
    
    for(int i=0; i<=M; i++)
        asset_file<<i*delta_t[1]<<" "<<(*s_4)[i]<<" "<<(*s_5)[i]<<" "<<(*s_6)[i]<<std::endl;
    
    asset_file.close();
    
    //frees memory
    gsl_rng_free(r);
    return 0;
    
}










