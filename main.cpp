#include <iostream>
#include <vector>

#include <math.h>

#include "headers/gnuplot-iostream.h"

using namespace std;


//Function to approximate y' = y
double dydx(double x, double y){
    return y;
}


// Compute yn+1 given xn, yn and step size h
double runge_kutta_4th_order(double x, double y, double h){

    double k1 = h * dydx(x,y);
    double k2 = h* dydx(x + (h/2), y + (k1/2));
    double k3 = h* dydx(x + (h/2), y + (k2/2));
    double k4 = h* dydx(x + h, y + k3);

    double y_next = y + (k1 + (2*k2) + (2*k3) + k4)/6.;

    return y_next;
}


//Compute yn+1 given xn, yn, h using adaptive RK 4th order method
double adaptive_runge_kutta_4th_order(double x, double y, double& h,double h_min, double h_max, double target_err, double s_factor)
{
    // Take one full step with rk4
    double y_full = runge_kutta_4th_order(x, y, h);

    // Take two half steps with rk4
    double h_half = h/2.;
    double y_half = runge_kutta_4th_order(x, y, h_half);
    y_half = runge_kutta_4th_order(x, y, h_half);

    //Calculate absolute and relative truncation error: take the min of two
    // Relative error
    double relative_error = fabs((y_half - y_full)/ y_half);
    // Absolute error
    double absolute_error = fabs(y_half - y_full);
    // Minimum of the two
    double error = relative_error < absolute_error? relative_error : absolute_error; 

    //Prevent rounding to zero
    if (error == 0.){ error = 10e-5;}

    // Calculate new step-length
    double h_new = h * pow( fabs(target_err / error), 0.2);
    
    //Check change limit of new h to be within change bounds : would have used conditional but there are 3 conditions
    if (h_new / h > s_factor){ h * s_factor; }
    
    else if (h / h_new < s_factor){ h /= s_factor; }

    else h = h_new;

    // Check if step size below allowed max step size. This prevents error from blowing up at boundary conditions
    h = fabs(h_max) < fabs(h) ? h_max * h / fabs(h) : h;
    
    // If h is less than minimum h, we are already doomed
    if (h < fabs(h_min)) { 
        cout<<"Unable to converge!";
        exit(1);
        }

    // Calculate the next step with new parameter h
    double y_next = runge_kutta_4th_order(x, y, h);

    return y_next;
}


int main(){

    Gnuplot gp;

    vector<double> rk4_yvector, adaptive_rk4_yvector;

    double x, y, h;
    double x_rk4, x_ark4, y_rk4, y_ark4, y_next_rk4, y_next_ark4, h_rk4, h_ark4;

    x = x_rk4 = x_ark4 = 0;
    y = y_rk4 = y_ark4 = 1;
    h = h_rk4 = h_ark4 = 0.005;

    double eval_for_x = 1.;

    for (int iter = 0; iter < 2000; iter++){
        
        y_next_rk4 = runge_kutta_4th_order(x_rk4, y_rk4, h_rk4);
        y_next_ark4 = adaptive_runge_kutta_4th_order(x_ark4, y_ark4, h_ark4, 0.0001, 0.01, 10e-3, 1.5);

        // rk4_xvector.push_back(x_rk4);
        // adaptive_rk4_xvector.push_back(x_ark4);

        rk4_yvector.push_back(y_next_rk4);
        adaptive_rk4_yvector.push_back(y_next_ark4);

        cout<< "Iteration: "<< iter << " | RK4 yn+1 = " << y_next_rk4 << " | Adaptive RK4 yn+1 = "<< y_next_ark4 <<"  || Adaptive h: "<< h_ark4<< endl;
        
        // Update x and y for next iteration    
        x_rk4 = x_rk4 + h_rk4;
        y_rk4 = y_next_rk4;
        
        x_ark4 = x_ark4 + h_ark4;
        y_ark4 = y_next_ark4;

        // if (y_rk4 > 2.71){
        //     y_rk4 = 2.7181;
        // }
        // Check and break when x = 1 for y = e = 2.7181..
        // if (x_rk4 > eval_for_x || x_ark4 > eval_for_x){
        if ( x_ark4 > eval_for_x){
            break;
        }
    }

    gp << "set term x11 title 'Runge-Kutta Differential Equation Solver'\n";
    // gp << "set xrange [1:2]\nset yrange [0:3]\n";
    gp << "plot '-' with points title 'RK4','-' with points title 'Adaptive RK4'\n";
    gp.send1d(rk4_yvector);
    gp.send1d(adaptive_rk4_yvector);
    // gp.send1d(boost::make_tuple( rk4_yvector, adaptive_rk4_yvector));

return 0;
}