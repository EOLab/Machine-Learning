#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#ifndef GARBAGE_H
#define GARBAGE_H
int garbage_num = 0;
void *garbage[3000] = { 0, };
#endif
#include <stdio.h>
#include <cmath>
#include <iostream>
#include <vector>
struct strDataType
{
	int*						pArrOfData;

	int*						pArrOfLabel;

	char*						pStrOfDataName;

	int							nCntOfLabel;
};

int*							returnArrOfLabel					(int *array,int nSize)
{
	int temp[8] = { 999, 999, 999, 999, 999, 999, 999, 999 };
	temp[0] = array[0];
	int j = 0;
	for (int i = 0; i < nSize; i++)
	{
		if (array[i] != temp[j])
		{
			int ind = j;
			while (ind >= 0)
			{
				if (array[i] == temp[ind])
				{
					ind = -1;
					break;
				}
				if (ind == 0)
				{
					j += 1;
					temp[j] = array[i];
					ind = -1;
					break;
				}
				ind--;
			}
		}
	}
	int cnt = 0;
	for (int i = 0; i < 8; i++)
	{
		if (temp[i] == 999)
		{
			break;
		}
		cnt++;
	}
	//int* label_array = (int*)malloc(cnt * sizeof(int));
	int* label_array = new int[cnt];
	garbage[garbage_num] = label_array;
	garbage_num++;
	for (int i = 0; i < cnt; i++)
	{
		label_array[i] = temp[i];
	}
	return label_array;
}

int								returNCntOfLabel					(int *array,int nSize)
{
	int temp[8] = { 999, 999, 999, 999, 999, 999, 999, 999 };
	temp[0] = array[0];
	int j = 0;
	for (int i = 0; i < nSize; i++)
	{
		if (array[i] != temp[j])
		{
			int ind = j;
			while (ind >= 0)
			{
				if (array[i] == temp[ind])
				{
					ind = -1;
					break;
				}
				if (ind == 0)
				{
					j += 1;
					temp[j] = array[i];
					ind = -1;
					break;
				}
				ind--;
			}
		}
	}
	int cnt = 0;
	for (int i = 0; i < 8; i++)
	{
		if (temp[i] == 999)
		{
			break;
		}
		cnt++;
	}
	return cnt;
}

struct							node
{
	std::vector<int>			vecListOfInstance;

	int							nNodeInstanceCnt = 0;

	int							nNodeNumber;

	int							nNumOfDataType = 999;

	int							nNumOfDataTypeLabel = 0;

	bool						bIsLast = false;

	bool						bIsUsed = false;

	struct node*				pUpperNode = NULL;

	std::vector<node*>			pChild;
};

double logB (double x)
{
	if (x == double(0))
	{
		return 0;
	}
	else
	{
		return log(x) / log(2.0);
	}
}

void RootInitializer(node** pArrOfNodePointer, node* pArrOfNode, int nDataCnt)
{
	for (int i = 0; i < 300; i++)
	{
		pArrOfNodePointer[i] = &pArrOfNode[i];
		pArrOfNodePointer[i]->nNodeNumber = i;
	}

	for (int i = 0; i < nDataCnt; i++)
	{
		pArrOfNodePointer[0]->vecListOfInstance.push_back(i);
	}
	pArrOfNodePointer[0]->nNumOfDataType = 0;
	pArrOfNodePointer[0]->nNodeInstanceCnt = int(pArrOfNodePointer[0]->vecListOfInstance.size());
	pArrOfNodePointer[0]->pUpperNode = NULL;
	pArrOfNodePointer[0]->bIsUsed = true;
}

double CalcEntropy(struct node** pListOfNodePointer, struct strDataType** pArrOfDataType, int nNumOfDtype, int nNodeNum)
{
	int *pSurvived = pArrOfDataType[0]->pArrOfData;
	int *pData = pArrOfDataType[nNumOfDtype]->pArrOfData;
	int *pLabel = pArrOfDataType[nNumOfDtype]->pArrOfLabel;
	int nCntOfLabel = pArrOfDataType[nNumOfDtype]->nCntOfLabel;
	int nArrTemp[8][3] = { 0, };

	for (int nIndex = 0; nIndex < pListOfNodePointer[nNodeNum]->nNodeInstanceCnt; nIndex++)
	{
		for (int nLabelIndex = 0; nLabelIndex < nCntOfLabel; nLabelIndex++)
		{
			if (pSurvived[pListOfNodePointer[nNodeNum]->vecListOfInstance[nIndex]] == 0 && pData[pListOfNodePointer[nNodeNum]->vecListOfInstance[nIndex]] == pLabel[nLabelIndex])
			{
				nArrTemp[nLabelIndex][0] += 1;
			}
			else if (pSurvived[pListOfNodePointer[nNodeNum]->vecListOfInstance[nIndex]] == 1 && pData[pListOfNodePointer[nNodeNum]->vecListOfInstance[nIndex]] == pLabel[nLabelIndex])
			{
				nArrTemp[nLabelIndex][1] += 1;
			}
		}

	}

	int nTotal = 0;
	for (int nLabelIndex = 0; nLabelIndex < nCntOfLabel; nLabelIndex++)
	{
		nArrTemp[nLabelIndex][2] = nArrTemp[nLabelIndex][0] + nArrTemp[nLabelIndex][1];
		nTotal += nArrTemp[nLabelIndex][2];
	}

	double dInfogain = 0;
	for (int j = 0; j < nCntOfLabel; j++)
	{
		if (nArrTemp[j][2] != 0 && nTotal != 0)
		{
			dInfogain += double(nArrTemp[j][2]) / double(nTotal) * (-1.0* double(nArrTemp[j][0]) / double(nArrTemp[j][2]) * logB(double(nArrTemp[j][0]) / double(nArrTemp[j][2])) + -1.0* double(nArrTemp[j][1]) / double(nArrTemp[j][2]) * logB(double(nArrTemp[j][1]) / double(nArrTemp[j][2])));

		}
	}
	return dInfogain;
}

int	CalcRootIGA(struct node** pListOfNodePointer, struct strDataType** pArrOfDataType)
{
	int nSurvCnt = 0;
	int nTotalCnt = pListOfNodePointer[0]->nNodeInstanceCnt;
	for (int nIndex = 0; nIndex < nTotalCnt; nIndex++)
	{
		if (pArrOfDataType[0]->pArrOfData[nIndex] == 1)
		{
			nSurvCnt++;
		}
	}
	int nDeathCnt = nTotalCnt - nSurvCnt;
	printf("Survival : %d, Death : %d\n", nSurvCnt, nDeathCnt);


	double dRootEntropy = -1.0 * double(nSurvCnt) / double(nTotalCnt) * logB(double(nSurvCnt) / double(nTotalCnt)) + -1.0 * double(nDeathCnt) / double(nTotalCnt) * logB(double(nDeathCnt) / double(nTotalCnt));
	double dArrIGA[6] = { 0, };
	for (int i = 1; i < 6; i++)
	{
		dArrIGA[i] = dRootEntropy - CalcEntropy(pListOfNodePointer, pArrOfDataType, i, 0);
		printf("at Root Node, Property No. %d : IGA : %lf\n", i, dArrIGA[i]);
	}


	double dTempMax = dArrIGA[1];
	for (int i = 1; i < 6; i++)
	{
		if (dArrIGA[i] > dTempMax) dTempMax = dArrIGA[i];
	}
	int nIndexOfMax = 1;
	while (dArrIGA[nIndexOfMax] != dTempMax)
	{
		nIndexOfMax++;
	}
	printf("Index of Maximum IGA : %d\n", nIndexOfMax);
	printf("--------------------------------------------\n");

	return nIndexOfMax;
}

int	CalcNodeIGA(struct node** pListOfNodePointer, struct strDataType** pArrOfDataType, int nNodeNum)
{
	std::vector<int> nIDX;
	for (int i = 1; i < 6; i++)
	{
		nIDX.push_back(i);
	}

	node* pRoot = pListOfNodePointer[nNodeNum];

	while (pRoot != NULL && pRoot->nNumOfDataType != 0 && nIDX.size() != 0)
	{
		for (unsigned int i = 0; i < nIDX.size(); i++)
		{
			if (nIDX[i] == pRoot->nNumOfDataType)
			{
				//printf("%d,", nIDX[i]);
				nIDX.erase(nIDX.begin() + i);
				break;
			}
		}
		pRoot = pRoot->pUpperNode;
	}
	//printf("\n");
	if (nIDX.size() == 0)
	{
		return -1;
	}
	else if (pListOfNodePointer[nNodeNum]->nNodeInstanceCnt == 0)
	{
		return -1;
	}
	else if (pListOfNodePointer[nNodeNum]->bIsLast == true)
	{
		return -1;
	}
	else
	{

		int nSurvCnt = 0;
		int nTotalCnt = pListOfNodePointer[nNodeNum]->nNodeInstanceCnt;
		for (int nIndex = 0; nIndex < nTotalCnt; nIndex++)
		{
			if (pArrOfDataType[0]->pArrOfData[pListOfNodePointer[nNodeNum]->vecListOfInstance[nIndex]] == 1)
			{
				nSurvCnt++;
			}
		}
		int nDeathCnt = nTotalCnt - nSurvCnt;
		double dRootEntropy = -1.0 * double(nSurvCnt) / double(nTotalCnt) * logB(double(nSurvCnt) / double(nTotalCnt)) + -1.0 * double(nDeathCnt) / double(nTotalCnt) * logB(double(nDeathCnt) / double(nTotalCnt));
		printf("at Node %d, Root Entropy : %lf\n", nNodeNum, dRootEntropy);
		//double *dArrIGA = (double*)malloc(sizeof(double) * nIDX.size());
		double *dArrIGA = new double[nIDX.size()];
		//garbage[garbage_num] = dArrIGA;
		//garbage_num++;
		int nZeroEntropy = 0;
		for (unsigned int i = 0; i < nIDX.size(); i++)
		{
			dArrIGA[i] = dRootEntropy - CalcEntropy(pListOfNodePointer, pArrOfDataType, nIDX[i], nNodeNum);
			if (dArrIGA[i] == 0)
			{
				nZeroEntropy++;
			}
			printf("at Node %d, Property No. %d : IGA : %lf\n", nNodeNum, nIDX[i], dArrIGA[i]);
		}
		if (nZeroEntropy == nIDX.size())
		{
			printf("--------------------------------------------\n");
			delete []dArrIGA;
			return -1;
		}
		else
		{
			int nNewEndingPrinciple = 0;
			double dTempMax = dArrIGA[0];
			for (unsigned int i = 0; i < nIDX.size(); i++)
			{
				if (dArrIGA[i] > dTempMax)
				{
					dTempMax = dArrIGA[i];
				}
				else if (dArrIGA[i] == dTempMax)
				{
					nNewEndingPrinciple++;
				}
			}
			if (nNewEndingPrinciple == nIDX.size() && nIDX.size() != 1)
			{
				printf("--------------------------------------------\n");
				delete[]dArrIGA;
				return -1;
			}
			else
			{
				int nIndexOfMax = 0;
				while (dArrIGA[nIndexOfMax] != dTempMax)
				{
					nIndexOfMax++;
				}
				printf("Index of Maximum IGA : %d\n", nIDX[nIndexOfMax]);
				printf("--------------------------------------------\n");
				delete[]dArrIGA;
				return nIDX[nIndexOfMax];
			}
		}
	}
}

void MakeBranches(struct node** pListOfNodePointer, struct strDataType** pArrOfDataType, int nNumOfMaxIGA, int nRootNum, int nLeafStartNum)
{
	if (pListOfNodePointer[nRootNum]->bIsLast != true && pListOfNodePointer[nRootNum]->nNodeInstanceCnt != 0 && nNumOfMaxIGA != -1)
	{
		node* pRoot = pListOfNodePointer[nRootNum];
		for (int i = 0; i < pArrOfDataType[nNumOfMaxIGA]->nCntOfLabel; i++)
		{
			pRoot->pChild.push_back(pListOfNodePointer[nLeafStartNum + i]);
			pRoot->pChild[i]->nNumOfDataType = nNumOfMaxIGA;
			pRoot->pChild[i]->pUpperNode = pRoot;
			pRoot->pChild[i]->bIsLast = false;
			pRoot->pChild[i]->bIsUsed = true;
			pRoot->pChild[i]->nNodeInstanceCnt = 0;
		}

		int* nArrInstance = pArrOfDataType[nNumOfMaxIGA]->pArrOfData;
		int* nArrDataLabel = pArrOfDataType[nNumOfMaxIGA]->pArrOfLabel;
		int* nArrSurvival = pArrOfDataType[0]->pArrOfData;

		for (int nDataIDX = 0; nDataIDX < pRoot->nNodeInstanceCnt; nDataIDX++)
		{
			for (int nLabelIDX = 0; nLabelIDX < pArrOfDataType[nNumOfMaxIGA]->nCntOfLabel; nLabelIDX++)
			{
				if (nArrInstance[pRoot->vecListOfInstance[nDataIDX]] == nArrDataLabel[nLabelIDX])
				{
					pRoot->pChild[nLabelIDX]->vecListOfInstance.push_back(pRoot->vecListOfInstance[nDataIDX]);
					break;
				}
			}
		}
		for (int nLabelIDX = 0; nLabelIDX < pArrOfDataType[nNumOfMaxIGA]->nCntOfLabel; nLabelIDX++)
		{
			pRoot->pChild[nLabelIDX]->nNodeInstanceCnt = int(pRoot->pChild[nLabelIDX]->vecListOfInstance.size());
			pRoot->pChild[nLabelIDX]->nNumOfDataTypeLabel = nArrDataLabel[nLabelIDX];
		}

		for (int nLabelIDX = 0; nLabelIDX < pArrOfDataType[nNumOfMaxIGA]->nCntOfLabel; nLabelIDX++)
		{
			int nTempSurvCnt = 0;
			for (int nChildCnt = 0; nChildCnt < pRoot->pChild[nLabelIDX]->nNodeInstanceCnt; nChildCnt++)
			{
				if (nArrSurvival[pRoot->pChild[nLabelIDX]->vecListOfInstance[nChildCnt]] == 1)
				{
					nTempSurvCnt++;
				}
			}
			if (nTempSurvCnt == pRoot->pChild[nLabelIDX]->nNodeInstanceCnt)
			{
				pRoot->pChild[nLabelIDX]->bIsLast = true;
			}
			else if (nTempSurvCnt == 0)
			{
				pRoot->pChild[nLabelIDX]->bIsLast = true;
			}
		}
	}
	else
	{
		pListOfNodePointer[nRootNum]->bIsLast = true;
	}
}

void MakeBranchesFromRoot(node** pArrOfNodePointer, strDataType** pArrOfDataType) {
	int nNumOfMaxIGA = CalcRootIGA(pArrOfNodePointer, pArrOfDataType);
	MakeBranches(pArrOfNodePointer, pArrOfDataType, nNumOfMaxIGA, 0, 1);

	for (int i = 1; i < 300; i++)
	{
		nNumOfMaxIGA = CalcNodeIGA(pArrOfNodePointer, pArrOfDataType, i);
		int nTemp = 0;
		for (int j = 0; j < 300; j++)
		{
			if (pArrOfNodePointer[j]->bIsUsed == false)
			{
				nTemp = j;
				break;
			}
		}
		MakeBranches(pArrOfNodePointer, pArrOfDataType, nNumOfMaxIGA, i, nTemp);
	}
}

void ShowAllNodes(struct node* root, struct strDataType** pArrOfDataType)
{
	if (root != NULL)
	{
		printf("Node ID : %d\n", root->nNodeNumber);
		printf("Property : %s\n", pArrOfDataType[root->nNumOfDataType]->pStrOfDataName);
		printf("Label Number : %d\n", root->nNumOfDataTypeLabel);
		printf("Count : %d\n", root->nNodeInstanceCnt);
		printf("Last Node : %s\n", root->bIsLast ? "true" : "false");
		printf("Instance :");
		for (int i = 0; i < root->nNodeInstanceCnt; i++)
		{
			printf("%d,", root->vecListOfInstance[i]);
		}
		printf("\n");

		if (root->pUpperNode != NULL)
		{
			printf("Upper Node : %d\n", root->pUpperNode->nNodeNumber);
		}
		for (unsigned int i = 0; i < root->pChild.size(); i++)
		{
			if (root->pChild[i] != NULL) {
				printf("Child[%d] : %d\n", i, root->pChild[i]->nNodeNumber);
			}
		}
		printf("--------------------------------------------\n");
		for (unsigned int i = 0; i < root->pChild.size(); i++)
		{
			if (root->pChild[i] != NULL) {
				ShowAllNodes(root->pChild[i], pArrOfDataType);
			}
		}
	}
}

void CalcSurvival(struct strDataType** pArrOfDataType, struct strDataType** pArrOfDataTypeTest, struct node* origin, int nSize)
{
	for (int i = 0; i < nSize; i++)
	{
		struct node* pTemp = origin;
		while (pTemp->bIsLast != true && pTemp != NULL && pTemp->pChild[0] != NULL)
		{
			for (unsigned int j = 0; j < pTemp->pChild.size(); j++)
			{
				if (pTemp->pChild[j] != NULL)
				{
					for (int k = 0; k < 5; k++)
					{
						if (pTemp->pChild[j]->nNumOfDataType == k + 1 && pTemp->pChild[j]->nNumOfDataTypeLabel == pArrOfDataTypeTest[k]->pArrOfData[i]) {
							pTemp = pTemp->pChild[j];
							break;
						}
					}
				}
			}
		}
		if (pTemp->nNodeInstanceCnt == 0)
		{
			printf("Unpredictable!\n");
		}
		else {
			int nSurvNum = 0;
			for (int l = 0; l < pTemp->nNodeInstanceCnt; l++)
			{
				if (pArrOfDataType[0]->pArrOfData[l] == 1) nSurvNum++;
			}
			printf("Person No. %d, Node %d, Survival Rate : %d/%d = %lf\n", i + 1, pTemp->nNodeNumber, nSurvNum, pTemp->nNodeInstanceCnt, 100.0* double(nSurvNum) / double(pTemp->nNodeInstanceCnt));
		}
	}
}