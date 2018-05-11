#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
//fscanf�� ������ �ذ����ִ� �κ�

#ifndef GARBAGE_H
#define GARBAGE_H
int nGarbageCNT = 0;
void *pArrGarbage[10] = { 0, };
#endif
//�����Ҵ� �� �������� �ּҸ� �����Ͽ� ���� ������ delete�����ֱ� ����

#define ATTRIB 12
#define NODENUM 100
#define DEPTH_CONFINE 5
#define IGA_LIMIT -1.0
/*�˰���� ���õ� �Ϻ� parameter���� ������ ����
ATTRIB : �˰��򿡼� ���� Ư������ ����
NODENUM : �� �˰��򿡼��� �����Ҵ��� Ȱ������ �ʱ� ������ �̸� ������ �������� Node�� �����ص�. �� ������ �� Node���� ����
DEPTH_CONFINE : Ʈ���� ���̰� �� ������ŭ �Ǿ��� �� �� �̻� Branch�� ������ �ʴ´�
IGA_LIMIT : �ش� �������� IGA�� ���� ��� �� �̻� Leaf Node�� ������ �ʴ´�. ������ ���� ���� ��� -1.0���� �д�.*/



#include <stdio.h>
#include <cmath>
#include <iostream>
#include <vector>
#include <algorithm>
#include "header.h"



void main()
{
	FILE *fp;
	fp = fopen("wq_train.csv", "r");

	std::vector<int> vLabel;
	vLabel.resize(1);
	std::vector< std::vector<double> > vData;
	vData.resize(ATTRIB);
	for (int i = 0; i < ATTRIB; i++)
	{
		vData[i].resize(1);
	}
	int nCNT = 1;
	while (fscanf(fp, "%d", &vLabel[nCNT-1]) != EOF)
	{

		vLabel.resize(nCNT + 1);
		for(int j = 0; j < ATTRIB; j++)
		{

			fscanf(fp, ",%lf", &vData[j][nCNT-1]);
			vData[j].resize(nCNT + 1);
		}
		nCNT++;
	}
	nCNT--;
	vLabel.pop_back();
	std::vector< std::vector<double> > vSort = vData;
	vSort.resize(ATTRIB);
	for (int i = 0; i < ATTRIB; i++)
	{
		vSort[i].pop_back();
		vData[i].pop_back();
		std::sort(vSort[i].begin(), vSort[i].end());
	}
	fclose(fp);
	//fscanf�� vector�� �̿��Ͽ� ������ �о���δ�. vLabel : �󺧰���(1 & 2)
	//vData[i][j] : i�� �� Ư������ 0~11, j�� �� �����ͷ� ���� �� ���������� 2000���� �����Ͱ� �ִ�.
	//vSort[i][j] : vData�� Sorting�� �����̴�.


	sData sWine;
	sWine.vLabel = vLabel;
	sWine.vData = vData;
	sWine.vSort = vSort;
	//�ռ� �޾ƿ� �����͵��� ����ü sData�� sWine�� �����Ͽ���.


	struct node pArrOfNode[NODENUM];
	RootInitializer(pArrOfNode, sWine, nCNT);
	MakeBranchesAuto(sWine, pArrOfNode);
	CalcEndNodeProb(sWine, pArrOfNode, NODENUM);
	//NODENUM���� node ���� �̸� �����صΰ� �̸� �� �Լ��� �־ Ʈ���̴��� �����Ѵ�.
	//RootInitializer�� root�� ��� �������� instance���� �־��ְ� ���Ǹ� ���� �� node�� ��ȣ�� �������ִ� ������ �Ѵ�.
	//MakeBranchesAuto�� �� node���� Information Gain�� ����� �� IGA�� �ִ��� ���� Ư���� ��ȣ(0~11)�� Threshold���� ����Ͽ� Root�κ��� �������� ������ش�.
	//CalcEndNodeProb�� �ϼ��� Ʈ���� ��� �� leaf ��忡 ���ؼ��� ��������/���������� Ȯ���� ����Ͽ� ����� ������ �����Ѵ�.



	FILE *fp2;
	fp2 = fopen("wq_test.csv", "r");
	std::vector<int> vLabelTe;
	vLabelTe.resize(1);
	std::vector< std::vector<double> > vDataTe;
	vDataTe.resize(ATTRIB);
	for (int i = 0; i < ATTRIB; i++)
	{
		vDataTe[i].resize(1);
	}
	int nCNT_Te = 1;
	while (fscanf(fp, "%d", &vLabelTe[nCNT_Te - 1]) != EOF)
	{
		vLabelTe.resize(nCNT_Te + 1);
		for (int j = 0; j < ATTRIB; j++)
		{

			fscanf(fp, ",%lf", &vDataTe[j][nCNT_Te - 1]);
			vDataTe[j].resize(nCNT_Te + 1);
		}
		nCNT_Te++;
	}
	nCNT_Te--;
	vLabelTe.pop_back();
	fclose(fp2);
	//fscanf�� vector�� �̿��Ͽ� test data set�� �о���δ�.



	sData sTest;
	sTest.vLabel = vLabelTe;
	sTest.vData = vDataTe;
	//�о���� �����͸� sTest�� �����Ѵ�.


	printf("Test set! ");
	CalcTestProb(sWine, sTest, pArrOfNode, nCNT_Te);
	printf("Train set! ");
	CalcTestProb(sWine, sWine, pArrOfNode, nCNT);
	//�׽�Ʈ���� ���� ��Ȯ���� ����Ѵ�.



	for (int i = 0; i < nGarbageCNT; i++)
	{
		delete[] pArrGarbage[i];
	}
	system("pause");
	//�����Ҵ�� �������� ��� delete�����ش�.
}











void RootInitializer(node* pArrOfNode, sData sWine, int nDataCnt)
{
	//��Ʈ ��� �� �ٸ� ������ �ʱ�ȭ�����ְ� ��Ʈ ��忡 ��� �ν��Ͻ��� ����ִ´�.
	for (int i = 0; i < NODENUM; i++)
	{
		pArrOfNode[i].nNodeNum = i;
	}

	for (int i = 0; i < nDataCnt; i++)
	{
		pArrOfNode[0].vecInstList.push_back(i);
	}
	pArrOfNode[0].nInstCnt = int(pArrOfNode[0].vecInstList.size());
	pArrOfNode[0].bIsUsed = true;
}
/*���Ǹ� ���� �� node�� ��ȣ�� �ο��Ѵ�.
root ��带 0������ �����ϰ� ��� Ʈ���̴׼��� �ν��Ͻ��� ��Ʈ�� ����ִ´�.*/

double logB(double x)
{
	//IGA ��꿡 �ʿ��� ���� 2�� �α� �Լ��� �����Ѵ�.
	if (abs(x - 0.0) < 0.001)
	{
		return 0.0;
	}
	else
	{
		return log(x) / log(2.0);
	}
}
//���� 2�� �α׸� ����ϱ� ���� �Լ��̴�. input�� 0�� ��� ���Ѵ�� �߻��Ͽ�
//�˰����� ������ ���� �����ϱ� ���� 0�� �����ϵ��� �ߴ�.

double CalcArrEntropy(int* cntArr)
{
	//������ ���� ���ҷ� �ϴ� �迭�� �Է����� �޾� ��Ʈ���Ǹ� ��ȯ�Ѵ�.
	if (cntArr[2] != 0)
	{
		double result = 0.0;
		for (int i = 0; i < 2; i++)
		{
			result += -1.0*double(cntArr[i]) / double(cntArr[2])*logB(double(cntArr[i]) / double(cntArr[2]));
		}
		return result;
	}
	else
	{
		return 0.0;
	}
}
//cntArr�� ũ�� 3�� �迭��, 0�� ��� : ���������� ��, 1�� ��� : ���������� ��, 2�� ��� : 0���� 1���� ������ �����Ǿ��ִ�.
//�� �迭 ���� �������� Entropy�� ����ϴ� �Լ��̴�. �θ� ��忡�� �ڽ� ����� ��Ʈ���Ǹ� ����� �� ���ȴ�.

void CalcIGA(sData sWine, node* pNodeArr, int nNode, std::vector<int>* nVecExclude)
{
	//Ư�� ��忡�� IGA�� ���� ū Ư���� Threshold���� ã�´�.
	if (nVecExclude == NULL)
	{
		printf("Node %d : CalcIGA Stop : Requested from ReturnExcludeVec!\n", nNode);
		pNodeArr[nNode].bIsLast = true;
	}
	//nVecExclude�� NULL�̶�� ���� ReturnExcludeVec�Լ����� �� �̻� Ʈ���� Ű�� �ʿ䰡 ���ٴ� ����� ������ ���̴�
	// = Ʈ���� ����� ������ų�, ��� Ư���� ����߰ų�, �ش� �Լ��� input node�� Ʈ���� leaf node���� ���
	else
	{
		if (pNodeArr[nNode].bIsLast == false && pNodeArr[nNode].bIsUsed == true)
		{
			int cntArr[3] = { 0, };
			for (int i = 0; i < pNodeArr[nNode].nInstCnt; i++)
			{
				if (sWine.vLabel[pNodeArr[nNode].vecInstList[i]] == 1)
				{
					cntArr[0]++;
				}
				else if (sWine.vLabel[pNodeArr[nNode].vecInstList[i]] == 2)
				{
					cntArr[1]++;
				}
			}
			cntArr[2] = cntArr[0] + cntArr[1];

			double dRootEntropy = CalcArrEntropy(cntArr);
			//��� ����� instance���� �м��Ͽ� �������ֿ� ���������� ���� ���� �� RootEntropy�� ����Ѵ�.
			int nMaxAtt = 0;
			double dIGAMax = 0.0;
			double dThrTemp = 0.0;
			int nExcludePos = 0;

			for (int nAtt = 0; nAtt < ATTRIB; nAtt++)
			{
				int nThrStart = 0;
				int nThrEnd = 1;
				while (nThrStart < sWine.vSort[nAtt].size() && nThrEnd < sWine.vSort[nAtt].size())
				{
					if (nExcludePos < nVecExclude->size() && nAtt == (*nVecExclude)[nExcludePos])
					{
						nExcludePos++;
						break;
					}
					int cntArr0[3] = { 0, };
					int cntArr1[3] = { 0, };
					int cntTot = 0;
					double dChildEnt = 0.0;
					double dIGA = 0.0;
					double dThreshold = 0.0;
					while (nThrEnd < sWine.vSort[nAtt].size())
					{
						if (abs(sWine.vSort[nAtt][nThrStart] - sWine.vSort[nAtt][nThrEnd]) > 0.001)
						{
							dThreshold = (sWine.vSort[nAtt][nThrStart] + sWine.vSort[nAtt][nThrEnd]) / 2.0;
							break;
						}
						else
						{
							nThrEnd++;
						}
					}
					//��� attribute�� ���� ��� ����� Threshold���� ���Ѵ�. �� ��, data �߿� ���� ��ġ�� ��찡 �� �����Ƿ� ���� ���� �ڵ带 ����
					//�ߺ��Ǵ� data���� skip�Ѵ�.
					for (int i = 0; i < pNodeArr[nNode].nInstCnt; i++)
					{
						if (sWine.vData[nAtt][pNodeArr[nNode].vecInstList[i]] < dThreshold)
						{
							if (sWine.vLabel[pNodeArr[nNode].vecInstList[i]] == 1)
							{
								cntArr0[0]++;
							}
							else if (sWine.vLabel[pNodeArr[nNode].vecInstList[i]] == 2)
							{
								cntArr0[1]++;
							}
						}
						else
						{
							if (sWine.vLabel[pNodeArr[nNode].vecInstList[i]] == 1)
							{
								cntArr1[0]++;
							}
							else if (sWine.vLabel[pNodeArr[nNode].vecInstList[i]] == 2)
							{
								cntArr1[1]++;
							}
						}

					}
					//�ش� Threshold�� �������� �з����� ���� �� �������� ���ڸ� ���ϰ�, �̸� ���� �Ʒ����� IGA�� ����غ���.
					cntArr0[2] = cntArr0[0] + cntArr0[1];
					cntArr1[2] = cntArr1[0] + cntArr1[1];
					cntTot = cntArr0[2] + cntArr1[2];

					dChildEnt = double(cntArr0[2]) / double(cntTot)*CalcArrEntropy(cntArr0) + double(cntArr1[2]) / double(cntTot)*CalcArrEntropy(cntArr1);
					dIGA = dRootEntropy - dChildEnt;
					if (dIGA > dIGAMax)
					{
						dIGAMax = dIGA;
						nMaxAtt = nAtt;
						dThrTemp = dThreshold;
					}
					//IGA�� �ִ밡 �� ���� IGA, Attribute(0~11), Threshold�� ã�� ���� �̸� �Ͻ������� �����ϰ� ��� ���Ѵ�.



					//printf("Attrib : %d, Thr Start IDX : %d, Thr End IDX : %d, IGA : %lf, Children are divided : %d,%d\n", nAtt, nThrStart, nThrEnd, dIGA, cntArr0[2], cntArr1[2]);
					nThrStart = nThrEnd + 1;
					nThrEnd = nThrStart + 1;
				}
			}
			printf("at %d, Attrib %d, Threshold : %lf, IGA %lf\n", nNode, nMaxAtt, dThrTemp, dIGAMax);
			pNodeArr[nNode].nAttNum = nMaxAtt;
			pNodeArr[nNode].dThreshold = dThrTemp;
			//IGA�� ���� Ŭ ���� �����͵��� ��� �� �ش� node ������ �������ش�.
			if (dIGAMax < IGA_LIMIT)
			{
				pNodeArr[nNode].bIsLast = true;
			}
			//IGA�� IGA����ġ�� ���� ���� ��� �� �̻� leaf node�� ������ �ʴ´�.
		}
		else if (pNodeArr[nNode].bIsLast == true)
		{
			printf("Node %d : CalcIGA Stop : Last Node!\n", nNode);
		}
		else if (pNodeArr[nNode].bIsUsed == false)
		{
			printf("Node %d : CalcIGA Stop : Undefined Node!\n", nNode);
		}
		//leaf node, �Ǵ� �̸� ����� NODENUM���� ��� �� ���� ��뿩�ΰ� �������� ���� ��带 ����ϰ� �� ��� �Լ��� �ߴܵǵ��� �Ѵ�.
	}
}


void MakeBranches(sData sWine, node* pNodeArr, int nRootNum, int nNode)
{
	//Ư�� ��忡�� Child Node�� ������ִ� ������ �Ѵ�.
	node *pRoot = &pNodeArr[nRootNum];
	if (pRoot->bIsLast == true)
	{
		printf("Node %d : MakeBranches Stop : Last Node!\n", nRootNum);
	}
	//����� ������ �а� leaf node���� ���θ� �Ǵ��� �Ŀ� �ƴ� ��� ������ ģ��.
	else
	{
		int nArrTemp[3] = { 0, };
		node *pTemp = &pNodeArr[nRootNum];
		for (int i = 0; i < pTemp->nInstCnt; i++)
		{
			if (sWine.vLabel[pTemp->vecInstList[i]] == 1)
			{
				nArrTemp[0]++;
			}
			else if (sWine.vLabel[pTemp->vecInstList[i]] == 2)
			{
				nArrTemp[1]++;
			}
		}
		nArrTemp[2] = nArrTemp[0] + nArrTemp[1];
		if (nArrTemp[2] == nArrTemp[0] || nArrTemp[2] == nArrTemp[1])
		{
			pTemp->bIsLast = true;
		}
		//Ȥ�ó� �ش� ��尡 leaf node���� �ٽ� �� �� üũ�Ѵ�. �� ��, ��� �ν��Ͻ��� �ϳ��� �󺧷θ� �̷������ �ƴ���, �ν��Ͻ��� ���� 0�� �ƴ���, 2���� ��� üũ�� �� �ִ�.
		else
		{
			pRoot->pLChild = &pNodeArr[nNode];
			pRoot->pLChild->pUpper = pRoot;
			pRoot->pRChild = &pNodeArr[nNode + 1];
			pRoot->pRChild->pUpper = pRoot;

			int nAtt = pRoot->nAttNum;
			double dThr = pRoot->dThreshold;
			for (int i = 0; i < pRoot->nInstCnt; i++)
			{
				if (sWine.vData[nAtt][pRoot->vecInstList[i]] < dThr)
				{
					pRoot->pLChild->vecInstList.push_back(pRoot->vecInstList[i]);
				}
				else
				{
					pRoot->pRChild->vecInstList.push_back(pRoot->vecInstList[i]);
				}
			}
			pRoot->pLChild->nInstCnt = pRoot->pLChild->vecInstList.size();
			pRoot->pRChild->nInstCnt = pRoot->pRChild->vecInstList.size();
			//��忡 ����� Ư��(0~11), Threshold�� �������� Left, Right Child Node�� instance���� �����ش�.
			if (pRoot->nInstCnt == pRoot->pLChild->nInstCnt)
			{
				pRoot->pLChild->bIsUsed = false;
				pRoot->bIsLast = true;
			}
			else if (pRoot->nInstCnt == pRoot->pRChild->nInstCnt)
			{
				pRoot->pRChild->bIsUsed = false;
				pRoot->bIsLast = true;
			}
			//�θ� ��忡�� �ڽ� ���� ��� �ν��Ͻ��� �Ѱ��ִ� ��츦 �����Ѵ�. ��Ȥ �߻��Ѵ�.
			else
			{
				pRoot->pLChild->bIsUsed = true;
				pRoot->pRChild->bIsUsed = true;
				int nArrTemp2[3] = { 0, };
				node *pTemp = pRoot->pLChild;
				for (int i = 0; i < pTemp->nInstCnt; i++)
				{
					if (sWine.vLabel[pTemp->vecInstList[i]] == 1)
					{
						nArrTemp2[0]++;
					}
					else if (sWine.vLabel[pTemp->vecInstList[i]] == 2)
					{
						nArrTemp2[1]++;
					}
				}
				nArrTemp2[2] = nArrTemp2[0] + nArrTemp2[1];
				if (nArrTemp2[2] == nArrTemp2[0] || nArrTemp2[2] == nArrTemp2[1])
				{
					pTemp->bIsLast = true;
				}

				int nArrTemp3[3] = { 0, };
				pTemp = pRoot->pRChild;
				for (int i = 0; i < pTemp->nInstCnt; i++)
				{
					if (sWine.vLabel[pTemp->vecInstList[i]] == 1)
					{
						nArrTemp3[0]++;
					}
					else if (sWine.vLabel[pTemp->vecInstList[i]] == 2)
					{
						nArrTemp3[1]++;
					}
				}
				nArrTemp3[2] = nArrTemp3[0] + nArrTemp3[1];
				if (nArrTemp3[2] == nArrTemp3[0] || nArrTemp3[2] == nArrTemp3[1])
				{
					pTemp->bIsLast = true;
				}
				//���� �ڽ� ��忡 ���� ��� �ν��Ͻ��� �ϳ��� �󺧷θ� �̷������ �ƴ���, �ν��Ͻ��� ���� 0�� �ƴ���, 2���� ��� üũ �� leaf node ���θ� ���Ѵ�.
			}
		}
	}

}

std::vector<int>* ReturnExcludeVec(node* pNodeArr, int nNode)
{
	//Threshold�� ����� ��, �̹� ���� ��忡�� �з� �������� ����� Ư���� ���ܽ�ų �� �ְ� ���� ��忡�� �з� �������� ����� Ư���� ���ҷ� �ϴ� ���͸� ��ȯ�ϴ� �Լ��̴�.
	if (pNodeArr[nNode].bIsLast == false && pNodeArr[nNode].bIsUsed == true)
	{
		std::vector<int>* nArr = new std::vector<int>;
		pArrGarbage[nGarbageCNT++] = nArr;

		node* pRoot = pNodeArr[nNode].pUpper;
		while (pRoot != NULL)
		{
			nArr->push_back(pRoot->nAttNum);
			pRoot = pRoot->pUpper;
		}
		if (nArr->size() == ATTRIB)
		{
			printf("No more attributes to use!\n");
			return NULL;
		}
		else if (nArr->size() >= DEPTH_CONFINE)
		{
			printf("The tree is deep enough!\n");
			return NULL;
		}
		//Ʈ���� DEPTH_CONFINE���� �� ����� ���, ��� Ư���� �� ����� ��� �ش� ���� leaf node���� �� �� �ֵ��� NULL�� �����Ѵ�.
		else
		{
			sort(nArr->begin(), nArr->end());
			//printf("at %d exclude :", nNode);
			//for (int i = 0; i < nArr->size(); i++)
			//{
			//	printf("%d,", (*nArr)[i]);

			//}
			//printf("\n");
			return nArr;
			//��� ���ܻ��׿� �ش����� ���� ���, sorting �� ���͸� ��ȯ�Ѵ�.
		}
	}
	else
	{
		//Ȥ�ó� leaf node�� ���� �� �Լ��� ������� ��� ����ó���� ���� NULL�� �����Ѵ�.
		//printf("Node %d is Last\n", nNode);
		return NULL;
	}
}

void MakeBranchesAuto(sData sWine, node* pNodeArr)
{
	//��� �������� �ڵ����� ����������� �ϴ� �Լ��̴�.
	std::vector<int>* p1 = NULL;

	for (int i = 0; i < NODENUM; i++)
	{
		p1 = ReturnExcludeVec(pNodeArr, i);
		if (pNodeArr[i].bIsLast == false)
		{
			CalcIGA(sWine, pNodeArr, i, p1);
			//CalcIGA�Լ��� ���� IGA�� ���� Ŭ ���� Ư���� Threshold�� ã�´�.
		}
		int nTemp = 0;
		for (int j = 0; j < NODENUM; j++)
		{
			//�� for���� ���� ������ ���� ����� ��ȣ�� ã�Ƴ��� �̸� child node ���� �� ����ϱ� �����̴�.
			if (pNodeArr[j].bIsUsed == false)
			{
				nTemp = j;
				break;
			}
		}
		if (pNodeArr[i].bIsLast == false)
		{
			MakeBranches(sWine, pNodeArr, i, nTemp);
			//��Ʈ ���κ��� child ��带 ��������.
		}
	}
}

void CalcEndNodeProb(sData sWine, node* pNodeArr, int nSize)
{
	//��ü ��� �� �ش� ��尡 leaf node�� ��쿡�� ���ؼ� ��������/���������� Ȯ���� ����ϰ� �ش� ��忡 �����Ѵ�.
	for (int i = 0; i < nSize; i++)
	{
		if (pNodeArr[i].bIsLast == true)
		{
			for (int j = 0; j < pNodeArr[i].nInstCnt; j++)
			{
				if (sWine.vLabel[pNodeArr[i].vecInstList[j]] == 1)
				{
					pNodeArr[i].nArrCnt[0]++;
				}
				else if (sWine.vLabel[pNodeArr[i].vecInstList[j]] == 2)
				{
					pNodeArr[i].nArrCnt[1]++;
				}
			}
			pNodeArr[i].nArrCnt[2] = pNodeArr[i].nArrCnt[1] + pNodeArr[i].nArrCnt[0];
			pNodeArr[i].dArrProb[0] = double(pNodeArr[i].nArrCnt[0]) / double(pNodeArr[i].nArrCnt[2]);
			pNodeArr[i].dArrProb[1] = double(pNodeArr[i].nArrCnt[1]) / double(pNodeArr[i].nArrCnt[2]);
		}
	}
}

void CalcTestProb(sData sWine, sData sTest, node* pNodeArr, int nDataCnt)
{
	//�׽�Ʈ ���� Ʈ���� �ְ� ��������/���������� Ȯ���� ����Ѵ�.
	int nCorrect = 0;
	for (int i = 0; i < nDataCnt; i++)
	{
		node* pRoot = &pNodeArr[0];
		while (pRoot->bIsLast == false)
		{
			int nAtt = pRoot->nAttNum;
			double dThr = pRoot->dThreshold;
			if (sTest.vData[nAtt][i] < dThr)
			{
				pRoot = pRoot->pLChild;
			}
			else
			{
				pRoot = pRoot->pRChild;
			}
		}
		//while���� ���� �׽�Ʈ ������ �ν��Ͻ��� leaf node�� ������.
		int nTemp = 0;
		double dTemp = 0.0;
		for (int j = 0; j < 2; j++)
		{
			if (pRoot->dArrProb[j] > dTemp)
			{
				dTemp = pRoot->dArrProb[j];
				nTemp = j;
			}
		}
		//��/�������� Ȯ�� �� ū ���� ã�´�.
		int nLabel = nTemp + 1;
		if (nLabel == sTest.vLabel[i])
		{
			nCorrect++;
		}
		//�󺧰� Ȯ���� ū �ε����� ���� �Ŀ� ��ġ�� ��� �´� ������ ������Ų��.
		//printf("%d, %d, %d\n", i, nLabel, sTest.vLabel[i]);

	}
	printf("%d out of %d are correct! Accuracy : %.2f %%\n", nCorrect, nDataCnt, double(nCorrect)*100.0/double(nDataCnt));
}