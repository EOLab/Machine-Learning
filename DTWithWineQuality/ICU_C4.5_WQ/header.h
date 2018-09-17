#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#ifndef GARBAGE_H
#define GARBAGE_H
int nGarbageCNT = 0;
void *pArrGarbage[10] = { 0, };
#endif

#include <stdio.h>
#include <cmath>
#include <iostream>
#include <vector>

struct sData
{
	std::vector<int>									vLabel;
	std::vector< std::vector<double> >					vData;
	std::vector< std::vector<double> >					vSort;
};

struct node
{
	std::vector<int>									vecInstList;
	int													nInstCnt = 0;
	int													nNodeNum;
	int													nAttNum = 0;
	int													nArrCnt[2] = { 0, };
	double												dArrProb[2] = { 0.0,0.0};
	double												dThreshold = 0.0;
	bool												bIsLast = false;
	bool												bIsUsed = false;
	node*												pUpper = NULL;
	node*												pLChild = NULL;
	node*												pRChild = NULL;
};

void						RootInitializer						(node* pArrOfNode, sData sWine, int nDataCnt);
double						logB								(double x);
double						CalcArrEntropy						(int* cntArr);
void						CalcIGA								(sData sWine, node* pNodeArr, int nNode, std::vector<int>* nVecExclude);
void						MakeBranches						(sData sWine, node* pNodeArr, int nRootNum, int nNode);
std::vector<int>*			ReturnExcludeVec					(node* pNodeArr, int nNode);
void						MakeBranchesAuto					(sData sWine, node* pNodeArr);
void						CalcEndNodeProb						(sData sWine, node* pNodeArr, int nSize);
void						CalcTestProb						(sData sWine, sData sTest, node* pNodeArr, int nDataCnt);