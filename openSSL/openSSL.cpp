// openSSL.cpp : 定義主控台應用程式的進入點。
//

#include "stdafx.h"

#include "openssl\ecdh.h"
#include "openssl\obj_mac.h"
#include "openssl\evp.h"

#include <stdio.h>



void handleErrors()
{
	printf("Error!!\n");
}



EC_KEY* get_peerkey_low()
{
	EC_KEY *key;

	/* Create an Elliptic Curve Key object and set it up to use the ANSI X9.62 Prime 256v1 curve */
	if(NULL == (key = EC_KEY_new_by_curve_name(NID_X9_62_prime256v1))) handleErrors();

	/* Generate the private and public key */
	if(1 != EC_KEY_generate_key(key)) handleErrors();

	return key;
}




unsigned char *ecdh_low(size_t *secret_len)
{
	EC_KEY *key, *peerkey;
	int field_size;
	unsigned char *secret;

	/* Create an Elliptic Curve Key object and set it up to use the ANSI X9.62 Prime 256v1 curve */
	if(NULL == (key = EC_KEY_new_by_curve_name(NID_X9_62_prime256v1))) handleErrors();

	/* Generate the private and public key */
	if(1 != EC_KEY_generate_key(key)) handleErrors();

	/* Get the peer's public key, and provide the peer with our public key -
	 * how this is done will be specific to your circumstances */
	peerkey = get_peerkey_low();



	/* Calculate the size of the buffer for the shared secret */
	field_size = EC_GROUP_get_degree(EC_KEY_get0_group(key));
	*secret_len = (field_size+7)/8;

	/* Allocate the memory for the shared secret */
	if(NULL == (secret = (unsigned char*)OPENSSL_malloc(*secret_len))) handleErrors();

	/* Derive the shared secret */
	*secret_len = ECDH_compute_key(secret, *secret_len, EC_KEY_get0_public_key(peerkey),
						key, NULL);


	for( int i = 0; i < *secret_len; i++ )
		printf( "%d\n", secret[i] );


	/* Clean up */
	EC_KEY_free(key);
	EC_KEY_free(peerkey);

	if(*secret_len <= 0)
	{
		OPENSSL_free(secret);
		return NULL;
	}

	return secret;
}




int _tmain(int argc, _TCHAR* argv[])
{
	size_t len = 32;
	ecdh_low( &len );
	
	system("PAUSE");
	return 0;
}

