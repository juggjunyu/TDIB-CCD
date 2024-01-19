# pragma once
#include "mathOps.h"
#include "triBezier.h"
#include "recBezier.h"
#include "recRationalBezier.h"
template<typename ParamObj1, typename ParamObj2>
static void generatePatchPair(ParamObj1 &CpPos1, ParamObj1 &CpVel1, ParamObj2 &CpPos2, ParamObj2 &CpVel2){
	std::default_random_engine randGenerator(5);
	// std::default_random_engine randGenerator(std::random_device());
    Vector3d dir;
    for(int dim=0; dim<3; dim++) dir[dim] = randNormal(randGenerator);
    dir.normalize();
    dir/=1.625;
    // std::cout<<dir;

    for (int i = 0; i < ParamObj1::cntCp; i++) {
        for(int dim=0; dim<3; dim++) CpPos1.ctrlp[i][dim] = randNormal(randGenerator);
        for(int dim=0; dim<3; dim++) CpVel1.ctrlp[i][dim] = randNormal(randGenerator);
        CpPos1.ctrlp[i]+=dir;
        CpVel1.ctrlp[i]-=dir;
	}
	for (int i = 0; i < ParamObj2::cntCp; i++) {
        for(int dim=0; dim<3; dim++) CpPos2.ctrlp[i][dim] = randNormal(randGenerator);
        for(int dim=0; dim<3; dim++) CpVel2.ctrlp[i][dim] = randNormal(randGenerator);
        CpPos2.ctrlp[i]-=dir;
        CpVel2.ctrlp[i]+=dir;
	}
}

template<typename ParamObj1, typename ParamObj2, typename ParamBound1, typename ParamBound2>
static double primitiveCheck(const ParamObj1 &CpPos1, const ParamObj1 &CpVel1, 
						const ParamObj2 &CpPos2, const ParamObj2 &CpVel2,
						const ParamBound1 &divUvB1, const ParamBound2 &divUvB2,
						const double upperTime = DeltaT){
	auto const ptPos1 = CpPos1.divideBezierPatch(divUvB1);
	auto const ptVel1 = CpVel1.divideBezierPatch(divUvB1);
	auto const ptPos2 = CpPos2.divideBezierPatch(divUvB2);
	auto const ptVel2 = CpVel2.divideBezierPatch(divUvB2);

	auto setAxes = [&] (std::vector<Vector3d>& axes) {
		
	};

	// std::cout<<"done!\n";
	std::vector<Vector3d> axes;
	// axes.clear();
	if(bbtype==BoundingBoxType::AABB){
		axes = {Vector3d::Unit(0), Vector3d::Unit(1), Vector3d::Unit(2)};
	}
	else if(bbtype==BoundingBoxType::DOP14){
		axes = {Vector3d::Unit(0), Vector3d::Unit(1), Vector3d::Unit(2), 
				Vector3d(1,1,1).normalized(), Vector3d(-1,1,1).normalized(), Vector3d(-1,-1,1).normalized()};
	}
	else if(bbtype==BoundingBoxType::OBB){
		Vector3d lu1 = (ptPos1[ParamObj1::cornerId(2)]-ptPos1[ParamObj1::cornerId(0)]
				+ptPos1[ParamObj1::cornerId(3)]-ptPos1[ParamObj1::cornerId(1)]).normalized();//u延展的方向
		Vector3d lv1 = (ptPos1[ParamObj1::cornerId(1)]-ptPos1[ParamObj1::cornerId(0)]
				+ptPos1[ParamObj1::cornerId(3)]-ptPos1[ParamObj1::cornerId(2)]);//v延展的方向
		lv1 = (lv1-lv1.dot(lu1)*lu1).eval();
		Vector3d ln1 = lu1.cross(lv1);

		Vector3d lu2 = (ptPos2[ParamObj2::cornerId(2)]-ptPos2[ParamObj2::cornerId(0)]
				+ptPos2[ParamObj2::cornerId(3)]-ptPos2[ParamObj2::cornerId(1)]).normalized();//u延展的方向
		Vector3d lv2 = (ptPos2[ParamObj2::cornerId(1)]-ptPos2[ParamObj2::cornerId(0)]
				+ptPos2[ParamObj2::cornerId(3)]-ptPos2[ParamObj2::cornerId(2)]);//v延展的方向
		lv2 = (lv2-lv2.dot(lu2)*lu2).eval();
		Vector3d ln2 = lu2.cross(lv2);

		axes = {lu1,lv1,ln1,lu2,lv2,ln2, 
			lu1.cross(lu2), lu1.cross(lv2), lu1.cross(ln2), 
			lv1.cross(lu2), lv1.cross(lv2), lv1.cross(ln2), 
			ln1.cross(lu2), ln1.cross(lv2), ln1.cross(ln2)};
	}
	std::vector<Array2d> feasibleIntvs;
	feasibleIntvs.clear();

	// auto AxisCheck=[&](const std::array<Vector3d, ParamObj1::cntCp>& p1, const std::array<Vector3d, ParamObj1::cntCp>& v1, 
	// 					const std::array<Vector3d, ParamObj2::cntCp>& p2, const std::array<Vector3d, ParamObj2::cntCp>& v2, 
	// 					const Vector3d& axis){
		// std::map<double, double> lines1, lines2;
		// std::map<double, double> ch1, ch2;
		// std::vector<double> pts1, pts2;
		// lines1.clear(); lines2.clear();
		// for(int i = 0; i < CpPos1.cntCp; i++) {
		// 	double k=v1[i].dot(axis), b=p1[i].dot(axis);
		// 	auto it = lines1.find(k);
		// 	if(it!=lines1.end()){
		// 		it->second = std::max(it->second, b);
		// 	}
		// 	else lines1[k]=b;
		// }
		// for(int i = 0; i < CpPos1.cntCp; i++) {
		// 	double k=-v2[i].dot(axis), b=-p2[i].dot(axis);
		// 	auto it = lines2.find(k);
		// 	if(it!=lines2.end()){
		// 		it->second = std::max(it->second, b);
		// 	}
		// 	else lines2[k]=b;
		// }
		// // auto CHCheck=[&](std::map<double,double> lineSet1, std::map<double,double> lineSet2)
		// // {
		// ch1.clear(); ch2.clear();
		// pts1.clear(); pts2.clear();
		// getCH(lines1, ch1, pts1, true, upperTime);
		// getCH(lines2, ch2, pts2, false, upperTime);//1/20
		// std::map<double,double,std::greater<double> > ch3;
		// ch3.clear();
		// for(const auto& it:ch2) {
		// 	ch3[-it.first]=-it.second;
		// }
		// const auto intvT = linearCHIntersect(ch1, ch3, pts1, pts2, upperTime);
		// if(intvT[0]!=-1)feasibleIntvs.push_back(intvT);
		// // };
		// // CHCheck(lines1, lines2);
		// // CHCheck(lines2, lines1);
	// };



	auto AxisCheck=[&](std::vector<Line> lines1, std::vector<Line> lines2){
        // for(auto & l:lines2)
        //     l.k = -l.k, l.b = -l.b;
        std::vector<Line> ch1, ch2;
        std::vector<double> pts1, pts2;
		ch1.clear(); ch2.clear();
		pts1.clear(); pts2.clear();

        getCH(lines1, ch1, pts2, true);
        getCH(lines2, ch2, pts2, false);
		// std::cout<<"getCHOK!\n";
        // for(auto & l:ch2)
        //     l.k = -l.k, l.b = -l.b;
        const auto intvT = linearCHIntersect(ch1, ch2, pts1, pts2);
		if(SHOWANS) std::cout<<intvT.transpose()<<"\n";
		if(DEBUG) std::cin.get();
        if(intvT[0]!=-1)feasibleIntvs.push_back(intvT);
	};

    for(const auto& axis:axes){
		std::vector<Line> ptLines1, ptLines2;
		ptLines1.clear(); ptLines2.clear();
		for(int i = 0; i < 16; i++) ptLines1.emplace_back(ptVel1[i].dot(axis), ptPos1[i].dot(axis));
		for(int i = 0; i < 16; i++) ptLines2.emplace_back(ptVel2[i].dot(axis), ptPos2[i].dot(axis));
		std::sort(ptLines1.begin(), ptLines1.end());
        std::sort(ptLines2.begin(), ptLines2.end());
        AxisCheck(ptLines1, ptLines2);
        AxisCheck(ptLines2, ptLines1);
	}
    // for(const auto& axis:axes)
    //     AxisCheck(ptPos2, ptVel2, ptPos1, ptVel1, axis);


	if(SHOWANS) std::cout<<"done!\n";

	if (feasibleIntvs.size()==0) return 0; //这意味着整段时间都有碰撞

	//无碰撞发生的并，剩下的就是有碰撞发生的
	std::sort(feasibleIntvs.begin(), feasibleIntvs.end(), 
		[](const Array2d& intv1, const Array2d& intv2){
			return (intv1(0)<intv2(0));
		});
	if(feasibleIntvs[0](0)>0) return 0;
	double minT = feasibleIntvs[0](1);
	for(int i=1;i<feasibleIntvs.size();i++)
		if(feasibleIntvs[i](0)<minT) //不能加等，因为无碰撞给的是开区间，如果有),(的情况加等号会把这个情况漏掉
			minT=std::max(minT, feasibleIntvs[i](1));
		else break;
	
	if(minT<upperTime)return minT;
	else return -1;
}

template<typename ParamObj1, typename ParamObj2, typename ParamBound1, typename ParamBound2>
static double solveCCD(const ParamObj1 &CpPos1, const ParamObj1 &CpVel1, 
						const ParamObj2 &CpPos2, const ParamObj2 &CpVel2,
						Array2d& uv1, Array2d& uv2, 
						const double upperTime = DeltaT) {
	struct PatchPair{
		ParamBound1 pb1;
		ParamBound2 pb2;
		double tLower;
		PatchPair(const ParamBound1& c1, const ParamBound2& c2, 
				double t = std::numeric_limits<double>::infinity()): pb1(c1), pb2(c2), tLower(t) {}
		bool operator<(PatchPair const &o) const { return tLower > o.tLower; }

		// double calcDist(const ParamObj1 &CpPos1, const ParamObj1 &CpVel1, 
		// 				const ParamObj2 &CpPos2, const ParamObj2 &CpVel2) const{
		// 	Vector3d const p1 = CpPos1.evaluatePatchPoint(pb1.centerParam());
		// 	Vector3d const v1 = CpVel1.evaluatePatchPoint(pb1.centerParam());
		// 	Vector3d const p2 = CpPos2.evaluatePatchPoint(pb2.centerParam());
		// 	Vector3d const v2 = CpVel2.evaluatePatchPoint(pb2.centerParam());
        // 	Vector3d const pt1=(v1*tLower+p1), pt2=(v2*tLower+p2);
		// 	return (pt2-pt1).norm();
		// }
		// double calcSquaredDist(const ParamObj1 &CpPos1, const ParamObj1 &CpVel1, 
		// 				const ParamObj2 &CpPos2, const ParamObj2 &CpVel2) const{
		// 	Vector3d const p1 = CpPos1.evaluatePatchPoint(pb1.centerParam());
		// 	Vector3d const v1 = CpVel1.evaluatePatchPoint(pb1.centerParam());
		// 	Vector3d const p2 = CpPos2.evaluatePatchPoint(pb2.centerParam());
		// 	Vector3d const v2 = CpVel2.evaluatePatchPoint(pb2.centerParam());
        // 	Vector3d const pt1=(v1*tLower+p1), pt2=(v2*tLower+p2);
		// 	return (pt2-pt1).squaredNorm();
		// }
		double calcL1Dist(const ParamObj1 &CpPos1, const ParamObj1 &CpVel1, 
						const ParamObj2 &CpPos2, const ParamObj2 &CpVel2) const{
			auto const ptPos1 = CpPos1.divideBezierPatch(pb1);
			auto const ptPos2 = CpPos2.divideBezierPatch(pb2);
			double d=0;
			for(int axis=0;axis<3;axis++){
				double maxv = ptPos1[0][axis], minv=maxv;
				for(int i = 1; i < ParamObj1::cntCp; i++) {
					maxv=std::max(maxv, ptPos1[i][axis]);
					minv=std::min(minv, ptPos1[i][axis]);
				}
				d=std::max(d,maxv-minv);
			}
			for(int axis=0;axis<3;axis++){
				double maxv = ptPos2[0][axis], minv=maxv;
				for(int i = 1; i < ParamObj2::cntCp; i++) {
					maxv=std::max(maxv, ptPos2[i][axis]);
					minv=std::min(minv, ptPos2[i][axis]);
				}
				d=std::max(d,maxv-minv);
			}
			return d;
		}
	};

	using steady_clock = std::chrono::steady_clock;
	using duration = std::chrono::duration<double>;
	const auto initialTime = steady_clock::now();

	std::priority_queue<PatchPair> heap;
	ParamBound1 initParam1;
	ParamBound2 initParam2;
	double colTime = primitiveCheck(CpPos1, CpVel1, CpPos2, CpVel2, initParam1, initParam2, upperTime);
	if (colTime>=0 && colTime<=upperTime)
		heap.emplace(initParam1, initParam2, colTime);
	// std::cout<<"done!\n";

	while (!heap.empty()) {
		auto const cur = heap.top();
		// std::cout << "patch1 : {" << cur.pb1.pMin.transpose()<<"; "<< cur.pb1.pMax.transpose()<<"; " <<"}\n" 
		// 	<< " patch2 : {" << cur.pb2.pMin.transpose()<<"; "<< cur.pb2.pMax.transpose()<<"; "<<"}\n";
		// std::cin.get();
		heap.pop();
		cnt++;
		if(DEBUG) std::cout<<cnt<<"\n";

		// Decide whether the algorithm converges
		// calcSquaredDist(cur) < MinSquaredDist || 
		if (cur.calcL1Dist(CpPos1, CpVel1, CpPos2, CpVel2) < MinL1Dist) {
		// if (calcSquaredDist(cur) < MinSquaredDist) {
			// std::cout << cur.pb1.centerParam() << std::endl;
			// std::cout << cur.pb2.centerParam() << std::endl;
			uv1 = cur.pb1.centerParam();
			uv2 = cur.pb2.centerParam();
			const auto endTime = steady_clock::now();
			std::cout << "min time: "<<  cur.tLower << "\nused seconds: " <<
				duration(endTime - initialTime).count()
				<< std::endl;
			return cur.tLower;
		}

		// Divide the current patch into four-to-four pieces
		for (int i = 0; i < 4; i++) {
			ParamBound1 divUvB1(cur.pb1.interpSubpatchParam(i));
			for (int j = 0; j < 4; j++) {
				ParamBound2 divUvB2(cur.pb2.interpSubpatchParam(j));
				colTime = primitiveCheck(CpPos1, CpVel1, CpPos2, CpVel2, divUvB1, divUvB2, upperTime);//maybe also need timeLB?
				if (colTime>=0 && colTime<=upperTime){
					heap.emplace(divUvB1, divUvB2, colTime);
				}
			}
		}
	}

	const auto endTime = steady_clock::now();
	std::cout << "used seconds: " <<
		duration(endTime - initialTime).count()
		<< std::endl;
	return -1;
}

// auto triGenerate = generatePatchPair<TriCubicBezier,TriCubicBezier>;
// auto triBezierCCD = solveCCD<TriCubicBezier,TriCubicBezier,TriParamBound,TriParamBound>;

// auto recGenerate = generatePatchPair<RecCubicBezier,RecCubicBezier>;
auto recBezierCCD = solveCCD<RecCubicBezier,RecCubicBezier,RecParamBound,RecParamBound>;

// auto triLinearCCD = solveCCD<TriLinearBezier,TriLinearBezier,TriParamBound,TriParamBound>;

// auto recRatBezierCCD = solveCCD<RecQuadRatBezier,RecQuadRatBezier,RecParamBound,RecParamBound>;