/* This is a sample implementation of a libssh based SSH server */
/*
Copyright 2003-2009 Aris Adamantiadis
This file is part of the SSH Library
You are free to copy this file, modify it in any way, consider it being public
domain. This does not apply to the rest of the library though, but it is
allowed to cut-and-paste working code from this file to any license of
program.
The goal is to show the API in action. It's not a reference on how terminal
clients must be made or how a client should react.
*/
//#include "config.h"
#include <libssh/libssh.h>
#include <libssh/server.h>
#include <libssh/sftp.h>

#ifdef HAVE_ARGP_H
#include <argp.h>
#endif
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#ifndef KEYS_FOLDER
#ifdef _WIN32
#define KEYS_FOLDER
#else
#define KEYS_FOLDER "/etc/ssh/"
#endif
#endif


#pragma comment(lib, "Ws2_32.lib")



#ifdef WITH_PCAP
const char *pcap_file="debug.server.pcap";
ssh_pcap_file pcap;
void set_pcap(ssh_session session);

void set_pcap(ssh_session session)
{
	if(!pcap_file)
		return;
	pcap = ssh_pcap_file_new();
	if(ssh_pcap_file_open(pcap,pcap_file) == SSH_ERROR)
	{
		printf("Error opening pcap file\n");
		ssh_pcap_file_free(pcap);
		pcap=NULL;
		return;
	}
	ssh_set_pcap_file(session,pcap);
}

void cleanup_pcap(void);
void cleanup_pcap()
{
	ssh_pcap_file_free(pcap);
	pcap=NULL;
}
#endif


static int auth_password(const char *user, const char *password)
{
	if(strcmp(user,"aris"))
		return 0;
	if(strcmp(password,"lala"))
		return 0;
	return 1; // authenticated
}


#ifdef HAVE_ARGP_H
const char *argp_program_version = "libssh server example "
SSH_STRINGIFY(LIBSSH_VERSION);
const char *argp_program_bug_address = "<libssh@libssh.org>";
/* Program documentation. */
static char doc[] = "libssh -- a Secure Shell protocol implementation";
/* A description of the arguments we accept. */
static char args_doc[] = "BINDADDR";
/* The options we understand. */
static struct argp_option options[] = {
{
.name = "port",
.key = 'p',
.arg = "PORT",
.flags = 0,
.doc = "Set the port to bind.",
.group = 0
},
{
.name = "hostkey",
.key = 'k',
.arg = "FILE",
.flags = 0,
.doc = "Set the host key.",
.group = 0
},
{
.name = "dsakey",
.key = 'd',
.arg = "FILE",
.flags = 0,
.doc = "Set the dsa key.",
.group = 0
},
{
.name = "rsakey",
.key = 'r',
.arg = "FILE",
.flags = 0,
.doc = "Set the rsa key.",
.group = 0
},
{
.name = "verbose",
.key = 'v',
.arg = NULL,
.flags = 0,
.doc = "Get verbose output.",
.group = 0
},
{NULL, 0, 0, 0, NULL, 0}
};
/* Parse a single option. */
static error_t parse_opt (int key, char *arg, struct argp_state *state) {
/* Get the input argument from argp_parse, which we
* know is a pointer to our arguments structure.
*/
ssh_bind sshbind = state->input;
switch (key) {
case 'p':
ssh_bind_options_set(sshbind, SSH_BIND_OPTIONS_BINDPORT_STR, arg);
break;
case 'd':
ssh_bind_options_set(sshbind, SSH_BIND_OPTIONS_DSAKEY, arg);
break;
case 'k':
ssh_bind_options_set(sshbind, SSH_BIND_OPTIONS_HOSTKEY, arg);
break;
case 'r':
ssh_bind_options_set(sshbind, SSH_BIND_OPTIONS_RSAKEY, arg);
break;
case 'v':
ssh_bind_options_set(sshbind, SSH_BIND_OPTIONS_LOG_VERBOSITY_STR, "3");
break;
case ARGP_KEY_ARG:
if (state->arg_num >= 1) {
/* Too many arguments. */
argp_usage (state);
}
ssh_bind_options_set(sshbind, SSH_BIND_OPTIONS_BINDADDR, arg);
break;
case ARGP_KEY_END:
if (state->arg_num < 1) {
/* Not enough arguments. */
argp_usage (state);
}
break;
default:
return ARGP_ERR_UNKNOWN;
}
return 0;
}


/* Our argp parser. */
static struct argp argp = {options, parse_opt, args_doc, doc, NULL, NULL, NULL};
#endif /* HAVE_ARGP_H */

int main(int argc, char **argv)
{

#ifdef WIN32
	WSADATA WSAData;
	if( WSAStartup( MAKEWORD(1, 0), &WSAData) != 0 ) 
	{
		printf("-WSAStartup failure: WSAGetLastError=%d\r\n", WSAGetLastError() );
		return 0;
	}
#endif

	ssh_session session;
	ssh_bind sshbind;
	ssh_message message;
	ssh_channel chan=0;

	char buf[2048];
	int auth=0;
	//int sftp=0;
	int i;
	int r;
	sshbind = ssh_bind_new();
	session = ssh_new();

	ssh_bind_options_set(sshbind, SSH_BIND_OPTIONS_DSAKEY, KEYS_FOLDER "ssh_host_dsa_key");
	ssh_bind_options_set(sshbind, SSH_BIND_OPTIONS_RSAKEY, KEYS_FOLDER "ssh_host_rsa_key");

	//ssh_bind_options_set( sshbind, SSH_BIND_OPTIONS_HOSTKEY, "192.168.0.17" );
	//ssh_bind_options_set( sshbind, SSH_BIND_OPTIONS_BINDPORT, "9999" );

	int verbosity = SSH_LOG_PROTOCOL;
	int port = 22;

	ssh_options_set(session, SSH_OPTIONS_HOST, "127.0.0.1");
	ssh_options_set(session, SSH_OPTIONS_LOG_VERBOSITY, &verbosity);
	ssh_options_set(session, SSH_OPTIONS_PORT, &port);

#ifdef HAVE_ARGP_H
/*
* Parse our arguments; every option seen by parse_opt will
* be reflected in arguments.
*/
	argp_parse (&argp, argc, argv, 0, 0, sshbind);
#else
	(void) argc;
	(void) argv;
#endif

#ifdef WITH_PCAP
	//set_pcap(session);
#endif


	chan = ssh_channel_new(session);

	/*if(ssh_bind_listen(sshbind)<0)
	{
		printf("Error listening to socket: %s\n",ssh_get_error(sshbind));
		return 1;
	}*/

	/*r = ssh_bind_accept(sshbind,session);
	if(r==SSH_ERROR)
	{
		printf("error accepting a connection : %s\n",ssh_get_error(sshbind));
		return 1;
	}*/


	sftp_session sftp;
	sftp = sftp_server_new( NULL,  NULL);

	int ret = sftp_server_init(sftp);
	if( ret < 0 )
		printf("error.");



	//ssh_disconnect(session);
	//ssh_bind_free(sshbind);
#ifdef WITH_PCAP
	cleanup_pcap();
#endif
	ssh_finalize();
	return 0;
}