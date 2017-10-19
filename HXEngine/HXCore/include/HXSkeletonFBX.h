#pragma once
#include "HXCommon.h"
#include <fbxsdk.h>
#include "HXISkeleton.h"
#include <map>

namespace HX3D
{
	struct HXJointPose 
	{
		FbxAMatrix mtVertexTransformMatrix;
	};

	struct HXJoint 
	{
		std::vector<HXJointPose*> vctJointPose;
	};

	struct HXSkeleton
	{
		HXSkeleton()
		{
			nJointCount = 0;
		}
		int nJointCount;
		std::vector<HXJoint*> vctJoint;
	};

	struct HXVertJointWeights 
	{
		HXVertJointWeights()
		{
			nAttachJointIndex = -1;
			fWeightBias = 0;
		}
		int nAttachJointIndex;
		float fWeightBias;
	};

	struct HXAnimation 
	{
		HXAnimation()
		{
			nKeyNums = 0;
		}
		int nKeyNums;
		HXSkeleton xSkeleton;
		std::map<int, std::vector<HXVertJointWeights>> mapVertJointInfo;	//#include <map>
	};

	class HXMeshFBX;
	class HXSkeletonFBX : public HXISkeleton
	{
	public:
		HXSkeletonFBX();
		~HXSkeletonFBX();

		void MatrixScale(FbxAMatrix& pMatrix, double pValue);
		void MatrixAddToDiagonal(FbxAMatrix& pMatrix, double pValue);
		void MatrixAdd(FbxAMatrix& pDstMatrix, FbxAMatrix& pSrcMatrix);

		FbxAMatrix GetPoseMatrix(FbxPose* pPose,
			int pNodeIndex);

		FbxAMatrix GetGlobalPosition(FbxNode* pNode,
			const FbxTime& pTime,
			FbxPose* pPose = NULL,
			FbxAMatrix* pParentGlobalPosition = NULL);

		FbxAMatrix GetGeometry(FbxNode* pNode);

		void ComputeClusterDeformation(FbxAMatrix& pGlobalPosition,
			FbxMesh* pMesh,
			FbxCluster* pCluster,
			FbxAMatrix& pVertexTransformMatrix,
			FbxTime pTime,
			FbxPose* pPose);

		void ComputeLinearDeformation(FbxAMatrix& pGlobalPosition,
			FbxMesh* pMesh,
			FbxTime& pTime,
			FbxVector4* pVertexArray,
			FbxPose* pPose);

		void ComputeSkinDeformation(FbxAMatrix& pGlobalPosition,
			FbxMesh* pMesh,
			FbxTime& pTime,
			FbxVector4* pVertexArray,
			FbxPose* pPose);

		void DrawMesh(FbxNode* pNode, FbxTime& pTime, FbxAnimLayer* pAnimLayer,
			FbxAMatrix& pGlobalPosition, FbxPose* pPose);

		void DrawNode(FbxNode* pNode,
			FbxTime& lTime,
			FbxAnimLayer * pAnimLayer,
			FbxAMatrix& pParentGlobalPosition,
			FbxAMatrix& pGlobalPosition,
			FbxPose* pPose);

		void DrawNodeRecursive(FbxNode* pNode, FbxTime& pTime, FbxAnimLayer* pAnimLayer,
			FbxAMatrix& pParentGlobalPosition,
			FbxPose* pPose);

		// The input index is corresponding to the array returned from GetAnimStackNameArray.
		bool SetCurrentAnimStack(int pIndex);

		//// virtual void Update();
		virtual HXISkeleton* Clone(HXMesh* pMesh);
		void Initial(HXMeshFBX* pMesh, FbxScene* pScene);

		virtual void UpdateAnimation();
		void LoadAnimationCurve();

		static bool IsHaveSkeletonAnimation(FbxScene* pScene);

		FbxArray<FbxString*> mAnimStackNameArray;
		FbxAnimLayer * mCurrentAnimLayer;
		FbxTime mFrameTime, mStart, mStop, mCurrentTime;
		FbxTime mCache_Start, mCache_Stop;
		FbxScene * mScene;
		HXMeshFBX* mMesh;
		double mLastTime;


		int nCurKeyframe;
		HXAnimation* pAnimation;
		FbxVector4* srcControlPoints;
		int nControlPointCount;
		int nSpeed;
		bool bAnimationLoaded;
	};
}
