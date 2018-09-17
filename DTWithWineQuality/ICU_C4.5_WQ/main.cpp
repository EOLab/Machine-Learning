#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
//fscanf의 오류를 해결해주는 부분

#ifndef GARBAGE_H
#define GARBAGE_H
int nGarbageCNT = 0;
void *pArrGarbage[10] = { 0, };
#endif
//동적할당 된 변수들의 주소를 저장하여 종료 직전에 delete시켜주기 위함

#define ATTRIB 12
#define NODENUM 100
#define DEPTH_CONFINE 5
#define IGA_LIMIT -1.0
/*알고리즘과 관련된 일부 parameter들은 값으로 지정
ATTRIB : 알고리즘에서 사용될 특성들의 갯수
NODENUM : 본 알고리즘에서는 동적할당을 활용하지 않기 때문에 미리 적당한 갯수들의 Node를 선언해둠. 이 변수는 그 Node들의 갯수
DEPTH_CONFINE : 트리의 깊이가 이 변수만큼 되었을 때 더 이상 Branch를 만들지 않는다
IGA_LIMIT : 해당 변수보다 IGA가 적을 경우 더 이상 Leaf Node를 만들지 않는다. 제한을 두지 않을 경우 -1.0으로 둔다.*/



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
	//fscanf와 vector를 이용하여 파일을 읽어들인다. vLabel : 라벨값들(1 & 2)
	//vData[i][j] : i는 각 특성으로 0~11, j는 각 데이터로 현재 이 예제에서는 2000개의 데이터가 있다.
	//vSort[i][j] : vData를 Sorting한 벡터이다.


	sData sWine;
	sWine.vLabel = vLabel;
	sWine.vData = vData;
	sWine.vSort = vSort;
	//앞서 받아온 데이터들을 구조체 sData의 sWine에 저장하였다.


	struct node pArrOfNode[NODENUM];
	RootInitializer(pArrOfNode, sWine, nCNT);
	MakeBranchesAuto(sWine, pArrOfNode);
	CalcEndNodeProb(sWine, pArrOfNode, NODENUM);
	//NODENUM개의 node 수를 미리 선언해두고 이를 각 함수에 넣어서 트레이닝을 진행한다.
	//RootInitializer는 root에 모든 데이터의 instance들을 넣어주고 편의를 위해 각 node의 번호를 지정해주는 역할을 한다.
	//MakeBranchesAuto는 각 node에서 Information Gain을 계산한 후 IGA가 최대일 때의 특성의 번호(0~11)과 Threshold값을 계산하여 Root로부터 가지들을 만들어준다.
	//CalcEndNodeProb은 완성된 트리의 노드 중 leaf 노드에 대해서만 적포도주/백포도주일 확률을 계산하여 노드의 정보에 저장한다.



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
	//fscanf와 vector를 이용하여 test data set을 읽어들인다.



	sData sTest;
	sTest.vLabel = vLabelTe;
	sTest.vData = vDataTe;
	//읽어들인 데이터를 sTest에 저장한다.


	printf("Test set! ");
	CalcTestProb(sWine, sTest, pArrOfNode, nCNT_Te);
	printf("Train set! ");
	CalcTestProb(sWine, sWine, pArrOfNode, nCNT);
	//테스트셋을 토대로 정확도를 계산한다.



	for (int i = 0; i < nGarbageCNT; i++)
	{
		delete[] pArrGarbage[i];
	}
	system("pause");
	//동적할당된 변수들을 모두 delete시켜준다.
}











void RootInitializer(node* pArrOfNode, sData sWine, int nDataCnt)
{
	//루트 노드 및 다른 노드들을 초기화시켜주고 루트 노드에 모든 인스턴스를 집어넣는다.
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
/*편의를 위해 각 node에 번호를 부여한다.
root 노드를 0번으로 지정하고 모든 트레이닝셋의 인스턴스를 루트에 집어넣는다.*/

double logB(double x)
{
	//IGA 계산에 필요한 밑이 2인 로그 함수를 구현한다.
	if (abs(x - 0.0) < 0.001)
	{
		return 0.0;
	}
	else
	{
		return log(x) / log(2.0);
	}
}
//밑이 2인 로그를 계산하기 위한 함수이다. input이 0일 경우 무한대로 발산하여
//알고리즘이 터지는 것을 방지하기 위해 0을 리턴하도록 했다.

double CalcArrEntropy(int* cntArr)
{
	//포도주 수를 원소로 하는 배열을 입력으로 받아 엔트로피를 반환한다.
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
//cntArr는 크기 3의 배열로, 0번 요소 : 적포도주의 수, 1번 요소 : 백포도주의 수, 2번 요소 : 0번과 1번의 합으로 구성되어있다.
//이 배열 값을 기준으로 Entropy를 계산하는 함수이다. 부모 노드에서 자식 노드의 엔트로피를 계산할 때 사용된다.

void CalcIGA(sData sWine, node* pNodeArr, int nNode, std::vector<int>* nVecExclude)
{
	//특정 노드에서 IGA가 가장 큰 특성과 Threshold값을 찾는다.
	if (nVecExclude == NULL)
	{
		printf("Node %d : CalcIGA Stop : Requested from ReturnExcludeVec!\n", nNode);
		pNodeArr[nNode].bIsLast = true;
	}
	//nVecExclude가 NULL이라는 것은 ReturnExcludeVec함수에서 더 이상 트리를 키울 필요가 없다는 사실을 전달한 것이다
	// = 트리가 충분히 깊어졌거나, 모든 특성을 사용했거나, 해당 함수의 input node가 트리의 leaf node였을 경우
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
			//대상 노드의 instance들을 분석하여 백포도주와 적포도주의 수를 구한 후 RootEntropy를 계산한다.
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
					//모든 attribute에 대해 모든 경우의 Threshold값을 구한다. 이 때, data 중에 값이 겹치는 경우가 꽤 많으므로 위와 같은 코드를 통해
					//중복되는 data들은 skip한다.
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
					//해당 Threshold를 기준으로 분류했을 때의 각 포도주의 숫자를 구하고, 이를 토대로 아래에서 IGA를 계산해본다.
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
					//IGA가 최대가 될 때의 IGA, Attribute(0~11), Threshold를 찾기 위해 이를 일시적으로 저장하고 계속 비교한다.



					//printf("Attrib : %d, Thr Start IDX : %d, Thr End IDX : %d, IGA : %lf, Children are divided : %d,%d\n", nAtt, nThrStart, nThrEnd, dIGA, cntArr0[2], cntArr1[2]);
					nThrStart = nThrEnd + 1;
					nThrEnd = nThrStart + 1;
				}
			}
			printf("at %d, Attrib %d, Threshold : %lf, IGA %lf\n", nNode, nMaxAtt, dThrTemp, dIGAMax);
			pNodeArr[nNode].nAttNum = nMaxAtt;
			pNodeArr[nNode].dThreshold = dThrTemp;
			//IGA가 가장 클 때의 데이터들을 출력 후 해당 node 변수에 저장해준다.
			if (dIGAMax < IGA_LIMIT)
			{
				pNodeArr[nNode].bIsLast = true;
			}
			//IGA가 IGA제한치를 넘지 못할 경우 더 이상 leaf node를 만들지 않는다.
		}
		else if (pNodeArr[nNode].bIsLast == true)
		{
			printf("Node %d : CalcIGA Stop : Last Node!\n", nNode);
		}
		else if (pNodeArr[nNode].bIsUsed == false)
		{
			printf("Node %d : CalcIGA Stop : Undefined Node!\n", nNode);
		}
		//leaf node, 또는 미리 선언된 NODENUM개의 노드 중 아직 사용여부가 정해지지 않은 노드를 사용하게 될 경우 함수가 중단되도록 한다.
	}
}


void MakeBranches(sData sWine, node* pNodeArr, int nRootNum, int nNode)
{
	//특정 노드에서 Child Node를 만들어주는 역할을 한다.
	node *pRoot = &pNodeArr[nRootNum];
	if (pRoot->bIsLast == true)
	{
		printf("Node %d : MakeBranches Stop : Last Node!\n", nRootNum);
	}
	//노드의 변수를 읽고 leaf node인지 여부를 판단한 후에 아닐 경우 가지를 친다.
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
		//혹시나 해당 노드가 leaf node인지 다시 한 번 체크한다. 이 때, 모든 인스턴스가 하나의 라벨로만 이루어진게 아닌지, 인스턴스의 수가 0이 아닌지, 2가지 모두 체크할 수 있다.
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
			//노드에 저장된 특성(0~11), Threshold를 기준으로 Left, Right Child Node에 instance들을 보내준다.
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
			//부모 노드에서 자식 노드로 모든 인스턴스를 넘겨주는 경우를 방지한다. 간혹 발생한다.
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
				//양쪽 자식 노드에 대해 모든 인스턴스가 하나의 라벨로만 이루어진게 아닌지, 인스턴스의 수가 0이 아닌지, 2가지 모두 체크 후 leaf node 여부를 정한다.
			}
		}
	}

}

std::vector<int>* ReturnExcludeVec(node* pNodeArr, int nNode)
{
	//Threshold를 계산할 때, 이미 상위 노드에서 분류 기준으로 사용한 특성은 제외시킬 수 있게 상위 노드에서 분류 기준으로 사용한 특성을 원소로 하는 벡터를 반환하는 함수이다.
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
		//트리가 DEPTH_CONFINE보다 더 깊어질 경우, 모든 특성을 다 사용한 경우 해당 노드는 leaf node임을 알 수 있도록 NULL을 리턴한다.
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
			//모든 예외사항에 해당하지 않을 경우, sorting 후 벡터를 반환한다.
		}
	}
	else
	{
		//혹시나 leaf node에 대해 본 함수를 사용했을 경우 예외처리를 위해 NULL을 리턴한다.
		//printf("Node %d is Last\n", nNode);
		return NULL;
	}
}

void MakeBranchesAuto(sData sWine, node* pNodeArr)
{
	//모든 가지들이 자동으로 만들어지도록 하는 함수이다.
	std::vector<int>* p1 = NULL;

	for (int i = 0; i < NODENUM; i++)
	{
		p1 = ReturnExcludeVec(pNodeArr, i);
		if (pNodeArr[i].bIsLast == false)
		{
			CalcIGA(sWine, pNodeArr, i, p1);
			//CalcIGA함수를 통해 IGA가 가장 클 때의 특성과 Threshold를 찾는다.
		}
		int nTemp = 0;
		for (int j = 0; j < NODENUM; j++)
		{
			//이 for문은 아직 사용되지 않은 노드의 번호를 찾아내고 이를 child node 생성 시 사용하기 위함이다.
			if (pNodeArr[j].bIsUsed == false)
			{
				nTemp = j;
				break;
			}
		}
		if (pNodeArr[i].bIsLast == false)
		{
			MakeBranches(sWine, pNodeArr, i, nTemp);
			//루트 노드로부터 child 노드를 만들어나간다.
		}
	}
}

void CalcEndNodeProb(sData sWine, node* pNodeArr, int nSize)
{
	//전체 노드 중 해당 노드가 leaf node일 경우에만 대해서 적포도주/백포도주일 확률을 계산하고 해당 노드에 저장한다.
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
	//테스트 셋을 트리에 넣고 적포도주/백포도주일 확률을 계산한다.
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
		//while문을 통해 테스트 데이터 인스턴스를 leaf node로 보낸다.
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
		//적/포도주일 확률 중 큰 것을 찾는다.
		int nLabel = nTemp + 1;
		if (nLabel == sTest.vLabel[i])
		{
			nCorrect++;
		}
		//라벨과 확률이 큰 인덱스를 비교한 후에 일치할 경우 맞는 갯수를 증가시킨다.
		//printf("%d, %d, %d\n", i, nLabel, sTest.vLabel[i]);

	}
	printf("%d out of %d are correct! Accuracy : %.2f %%\n", nCorrect, nDataCnt, double(nCorrect)*100.0/double(nDataCnt));
}