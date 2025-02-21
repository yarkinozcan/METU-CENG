/********************************************************
 * Kernels to be optimized for the CS:APP Performance Lab
 ********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "defs.h"
/*
 * Please fill in the following team_t struct
 */
team_t team = {

        "e2580835",      /* First student ID */
        "Yarkın ÖZCAN",       /* First student name */

};


/********************
 * NORMALIZATION KERNEL
 ********************/

/****************************************************************
 * Your different versions of the normalization functions go here
 ***************************************************************/

/*
 * naive_normalize - The naive baseline version of convolution
 */
char naive_normalize_descr[] = "naive_normalize: Naive baseline implementation";
void naive_normalize(int dim, float *src, float *dst) {
    float min, max;
    int i, j;
    min = src[0];
    max = src[0];

    for (i = 0; i < dim; i++) {
        for (j = 0; j < dim; j++) {
	
            if (src[RIDX(i, j, dim)] < min) {
                min = src[RIDX(i, j, dim)];
            }
            if (src[RIDX(i, j, dim)] > max) {
                max = src[RIDX(i, j, dim)];
            }
        }
    }

    for (i = 0; i < dim; i++) {
        for (j = 0; j < dim; j++) {
            dst[RIDX(i, j, dim)] = (src[RIDX(i, j, dim)] - min) / (max - min);
        }
    }
}

/*
 * normalize - Your current working version of normalization
 * IMPORTANT: This is the version you will be graded on
 */
char normalize_descr[] = "Normalize: Current working version";
void normalize(int dim, float *src, float *dst)
{
    float j, l, range, inverse_range, val, min = src[0], max = src[0];
    int dim_sq = dim * dim;
    float *src_end = src + dim_sq;
    int i;	

    for (i = 1; i < dim_sq; i++) {
        val = src[i];      
        if (val > max) max = val;        
        else if (val < min) min = val;
    }


    range = max - min;
    inverse_range = 1.0f / range;

    while (src < src_end) {
        j = src[0];
        l = src[1];
        dst[0]  = (j - min) * inverse_range;
        dst[1]  = (l - min) * inverse_range;
        src += 2;
        j = src[0];
        l = src[1];                         
        dst[2]  = (j - min) * inverse_range;
        dst[3]  = (l - min) * inverse_range;
        src += 2;
	dst += 4;
    }  
}


/*********************************************************************
 * register_normalize_functions - Register all of your different versions
 *     of the normalization functions  with the driver by calling the
 *     add_normalize_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.
 *********************************************************************/

void register_normalize_functions() {
    add_normalize_function(&naive_normalize, naive_normalize_descr);
    add_normalize_function(&normalize, normalize_descr);
    /* ... Register additional test functions here */
}




/************************
 * KRONECKER PRODUCT KERNEL
 ************************/

/********************************************************************
 * Your different versions of the kronecker product functions go here
 *******************************************************************/

/*
 * naive_kronecker_product - The naive baseline version of k-hop neighbours
 */
char naive_kronecker_product_descr[] = "Naive Kronecker Product: Naive baseline implementation";
void naive_kronecker_product(int dim1, int dim2, float *mat1, float *mat2, float *prod) {
    int i, j, k, l;
    for (i = 0; i < dim1; i++) {
        for (j = 0; j < dim1; j++) {
            for (k = 0; k < dim2; k++) {
                for (l = 0; l < dim2; l++) {
                    prod[RIDX(i, k, dim2) * (dim1 * dim2) + RIDX(j, l, dim2)] = mat1[RIDX(i, j, dim1)] * mat2[RIDX(k, l, dim2)];
                }
            }
        }
    }
}



/*
 * kronecker_product - Your current working version of kronecker_product
 * IMPORTANT: This is the version you will be graded on
 */
char kronecker_product_descr[] = "Kronecker Product: Current working version";
void kronecker_product(int dim1, int dim2, float *mat1, float *mat2, float *prod) {
    int dim_prod = dim1 * dim2;  
    int dim_temp = dim2 * dim_prod; 
    int dim_last_temp = dim_prod - dim2;
    int dim_last_temp2 = (dim_last_temp + dim2) * dim2;
    int dim1_increment = 0;
    int i, j, dim2_increment;
    float *prod_ptr_end, *prod_ptr_temp, *prod_ptr, *mat2_ptr;
    float scalar;

    for (i = 0; i < dim1; i++) {
        dim2_increment = 0;
        for (j = 0; j < dim1; j++) {
            scalar = mat1[j];
            prod_ptr = prod + dim1_increment + dim2_increment;
            mat2_ptr = mat2;
            for (prod_ptr_end = prod_ptr + dim_last_temp2; prod_ptr < prod_ptr_end;prod_ptr += dim_last_temp) {
                for (prod_ptr_temp = prod_ptr + dim2; prod_ptr < prod_ptr_temp; prod_ptr += 4, mat2_ptr += 4) {
                    prod_ptr[0] = scalar * mat2_ptr[0];
                    prod_ptr[1] = scalar * mat2_ptr[1];
                    prod_ptr[2] = scalar * mat2_ptr[2];
                    prod_ptr[3] = scalar * mat2_ptr[3];
                }
            }
            dim2_increment += dim2;
        }
        dim1_increment += dim_temp;
        mat1 += dim1;
    }
}


/******************************************************************************
 * register_kronecker_product_functions - Register all of your different versions
 *     of the kronecker_product with the driver by calling the
 *     add_kronecker_product_function() for each test function. When you run the
 *     driver program, it will test and report the performance of each
 *     registered test function.  
 ******************************************************************************/

void register_kronecker_product_functions() {
    add_kronecker_product_function(&naive_kronecker_product, naive_kronecker_product_descr);
    add_kronecker_product_function(&kronecker_product, kronecker_product_descr);
    /* ... Register additional test functions here */
}

