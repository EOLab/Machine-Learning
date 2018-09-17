#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#ifndef GARBAGE_H
#define GARBAGE_H
int garbage_num = 0;
void *garbage[50] = { 0, };
#endif
#include <stdio.h>
#include <cmath>
#include <iostream>
#include <vector>
#include "id3.h"



void main() {





	FILE *fp;
	fp = fopen("train_mod.csv", "r");
	int nArrSurvival[1000] = { 0, };
	int nArrPclass[1000] = { 0, };
	int nArrSex[1000] = { 0, };
	int nArrEmbarked[1000] = { 0, };
	int nArrSibsp[1000] = { 0, };
	int nArrParch[1000] = { 0, };
	int nDataCnt = 0;
	while (fscanf(fp, "%d,%d,%d,%d,%d,%d\n", &nArrSurvival[nDataCnt], &nArrPclass[nDataCnt], &nArrSex[nDataCnt], &nArrEmbarked[nDataCnt], &nArrSibsp[nDataCnt], &nArrParch[nDataCnt]) != EOF)
	{
		nDataCnt++;
	}
	fclose(fp);
	strDataType surv = { nArrSurvival, returnArrOfLabel(nArrSurvival,nDataCnt), "survived",returNCntOfLabel(nArrSurvival,nDataCnt) };
	strDataType Pcla = { nArrPclass, returnArrOfLabel(nArrPclass,nDataCnt), "Pclass" ,returNCntOfLabel(nArrPclass,nDataCnt) };
	strDataType Se = { nArrSex, returnArrOfLabel(nArrSex,nDataCnt), "Sex",returNCntOfLabel(nArrSex,nDataCnt) };
	strDataType Emb = { nArrEmbarked, returnArrOfLabel(nArrEmbarked,nDataCnt), "Embarked" ,returNCntOfLabel(nArrEmbarked,nDataCnt) };
	strDataType Sib = { nArrSibsp, returnArrOfLabel(nArrSibsp,nDataCnt), "Sibsp" ,returNCntOfLabel(nArrSibsp,nDataCnt) };
	strDataType Par = { nArrParch, returnArrOfLabel(nArrParch,nDataCnt), "Parch" ,returNCntOfLabel(nArrParch,nDataCnt) };
	struct strDataType *pArrOfDataType[6] = { &surv, &Pcla, &Se, &Emb ,&Sib, &Par };






	struct node *pArrOfNodePointer[300];
	struct node pArrOfNode[300];

	RootInitializer(pArrOfNodePointer, pArrOfNode, nDataCnt);

	MakeBranchesFromRoot(pArrOfNodePointer, pArrOfDataType);

	ShowAllNodes(pArrOfNodePointer[0], pArrOfDataType);






	FILE *fp2;
	fp2 = fopen("test_mod.csv", "r");
	int nArrSurvival2[500] = { 0, };
	int nArrPclass2[500] = { 0, };
	int nArrSex2[500] = { 0, };
	int nArrEmbarked2[500] = { 0, };
	int nArrSibsp2[500] = { 0, };
	int nArrParch2[500] = { 0, };
	int nTestCnt = 0;
	while (fscanf(fp2, "%d,%d,%d,%d,%d,%d\n", &nArrSurvival2[nTestCnt], &nArrPclass2[nTestCnt], &nArrSex2[nTestCnt], &nArrEmbarked2[nTestCnt], &nArrSibsp2[nTestCnt], &nArrParch2[nTestCnt]) != EOF)
	{
		nTestCnt++;
	}
	fclose(fp2);
	strDataType surv2 = { nArrSurvival2, returnArrOfLabel(nArrSurvival2,nTestCnt), "survived",returNCntOfLabel(nArrSurvival2,nTestCnt) };
	strDataType Pcla2 = { nArrPclass2, returnArrOfLabel(nArrPclass2,nTestCnt), "Pclass" ,returNCntOfLabel(nArrPclass2,nTestCnt) };
	strDataType Se2 = { nArrSex2, returnArrOfLabel(nArrSex2,nTestCnt), "Sex",returNCntOfLabel(nArrSex2,nTestCnt) };
	strDataType Emb2 = { nArrEmbarked2, returnArrOfLabel(nArrEmbarked2,nTestCnt), "Embarked" ,returNCntOfLabel(nArrEmbarked2,nTestCnt) };
	strDataType Sib2 = { nArrSibsp2, returnArrOfLabel(nArrSibsp2,nTestCnt), "Sibsp" ,returNCntOfLabel(nArrSibsp2,nTestCnt) };
	strDataType Par2 = { nArrParch2, returnArrOfLabel(nArrParch2,nTestCnt), "Parch" ,returNCntOfLabel(nArrParch2,nTestCnt) };
	struct strDataType *pArrOfDataTypeTest[6] = { &surv2, &Pcla2, &Se2, &Emb2 ,&Sib2, &Par2 };
	





	CalcSurvival(pArrOfDataType, pArrOfDataTypeTest, pArrOfNodePointer[0], nTestCnt);






	for (int i = 0; i < garbage_num; i++) {
		delete []garbage[garbage_num];
	}






	system("pause");
}