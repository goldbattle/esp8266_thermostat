/*
 * ESP8266 Thermostat System
 * Copyright (C) 2021 Patrick Geneva
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */



#include <stdlib.h>
#include <math.h>


// Original code is from this stackoverflow 
// https://stackoverflow.com/questions/5083465/fast-efficient-least-squares-fit-algorithm-in-c
bool linreg(int n, const float x[], const float y[], float& m, float& b, float& r) {
    
    float sumx = 0.0; /* sum of x     */
    float sumx2 = 0.0; /* sum of x**2  */
    float sumxy = 0.0; /* sum of x * y */
    float sumy = 0.0; /* sum of y     */
    float sumy2 = 0.0; /* sum of y**2  */

    for (int i=0;i<n;i++){ 
        sumx += x[i];       
        sumx2 += x[i]*x[i];  
        sumxy += x[i]*y[i];
        sumy += y[i];      
        sumy2 += y[i]*y[i]; 
    }

    // Check if we can solve it
    // Singular matrix means we can't solve the problem.
    float denom = (n * sumx2 - sumx*sumx);
    if (denom == 0) {
        m = 1;
        b = 0;
        if (r) r = 0;
        return false;
    }

    // Calculate slope and intercept
    m = (n * sumxy  -  sumx * sumy) / denom;
    b = (sumy * sumx2  -  sumx * sumxy) / denom;

    // Compute correlation coeff
    r = (sumxy - sumx * sumy / n) / sqrt((sumx2 - (sumx*sumx)/n) * (sumy2 - (sumy*sumy)/n));
    return true;

}