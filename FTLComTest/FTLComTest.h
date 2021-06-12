#pragma once

#include "resource.h"
#include "..\\clsEDC.h"

using namespace std;

void _MyDoEvents();
void _MySleep(int islp);


int UTEDCAPIINI();
int UTEDCAPICOMTEST();
int UTEDCAPICALL_T703(int PayAmt);
int UTEDCAPICALL_T704(int PayAmt);
int UTEDCAPICALL_T705(int PayAmt);
int UTEDCAPICALL_T707(int PayAmt);
int UTEDCAPICALL_T708(int PayAmt);
int UTEDCAPICALL_T709(int PayAmt);

int UTEDCAPICALL_SIG_T710(int PayAmt);
int UTEDCAPICALL_SIG_T711(int PayAmt);

int UTEDCAPICALL_DBtn3(int PayAmt);



