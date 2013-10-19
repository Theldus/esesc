/*****************************************************************************
 *                                McPAT
 *                      SOFTWARE LICENSE AGREEMENT
 *            Copyright 2009 Hewlett-Packard Development Company, L.P.
 *                          All Rights Reserved
 *
 * Permission to use, copy, and modify this software and its documentation is
 * hereby granted only under the following terms and conditions.  Both the
 * above copyright notice and this permission notice must appear in all copies
 * of the software, derivative works or modified versions, and any portions
 * thereof, and both notices must appear in supporting documentation.
 *
 * Any User of the software ("User"), by accessing and using it, agrees to the
 * terms and conditions set forth herein, and hereby grants back to Hewlett-
 * Packard Development Company, L.P. and its affiliated companies ("HP") a
 * non-exclusive, unrestricted, royalty-free right and license to copy,
 * modify, distribute copies, create derivate works and publicly display and
 * use, any changes, modifications, enhancements or extensions made to the
 * software by User, including but not limited to those affording
 * compatibility with other hardware or software, but excluding pre-existing
 * software applications that may incorporate the software.  User further
 * agrees to use its best efforts to inform HP of any such changes,
 * modifications, enhancements or extensions.
 *
 * Correspondence should be provided to HP at:
 *
 * Director of Intellectual Property Licensing
 * Office of Strategy and Technology
 * Hewlett-Packard Company
 * 1501 Page Mill Road
 * Palo Alto, California  94304
 *
 * The software may be further distributed by User (but not offered for
 * sale or transferred for compensation) to third parties, under the
 * condition that such third parties agree to abide by the terms and
 * conditions of this license.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" WITH ANY AND ALL ERRORS AND DEFECTS
 * AND USER ACKNOWLEDGES THAT THE SOFTWARE MAY CONTAIN ERRORS AND DEFECTS.
 * HP DISCLAIMS ALL WARRANTIES WITH REGARD TO THE SOFTWARE, INCLUDING ALL
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS.   IN NO EVENT SHALL
 * HP BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES
 * OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER ACTION, ARISING
 * OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THE SOFTWARE.
 *
 ***************************************************************************/

#ifndef GPU_H_
#define GPU_H_

#include "XML_Parse.h"
#include "logic.h"
#include "parameter.h"
#include "array.h"
#include "interconnect.h"
#include "basic_components.h"
#include "sharedcache.h"


class InstFetchUG :public Component {
  public:

	ParseXML *XML;
	int  ithSM;
	int  ithLane;
	InputParameter interface_ip;
	CoreDynParam  coredynp;
	double clockRate,executionTime;
	double scktRatio, chip_PR_overhead, macro_PR_overhead;
	ArrayST * IB;

	InstFetchUG(ParseXML *XML_interface, int ithSM, int ithLane_, InputParameter* interface_ip_,const CoreDynParam & dyn_p_);
	void computeEnergy(bool is_tdp=true);
	~InstFetchUG();
};



class LoadStoreUG :public Component {
  public:

	ParseXML *XML;
	int  ithSM;
	int  ithLane;
	InputParameter interface_ip;
	CoreDynParam  coredynp;
	NoCParam  nocdynp;
	double clockRate,executionTime;
	double scktRatio, chip_PR_overhead, macro_PR_overhead;
	double lsq_height;
	ArrayST * LSQ;//it is actually the store queue but for inorder processors it serves as both loadQ and StoreQ

	LoadStoreUG(ParseXML *XML_interface, int ithSM, int ithLane_, InputParameter* interface_ip_,const CoreDynParam & dyn_p_);
	void computeEnergy(bool is_tdp=true);
	~LoadStoreUG();
};

class RegFUG :public Component {
  public:

	ParseXML *XML;
	int  ithSM;
	int  ithLane;
	InputParameter interface_ip;
	CoreDynParam  coredynp;
	double clockRate,executionTime;
	double scktRatio, chip_PR_overhead, macro_PR_overhead;
	double int_regfile_height, fp_regfile_height;
	ArrayST * IRF;

	RegFUG(ParseXML *XML_interface, int ithSM, int ithLane_, InputParameter* interface_ip_,const CoreDynParam & dyn_p_);
	void computeEnergy(bool is_tdp=true);
	~RegFUG();
};

class EXECUG :public Component {
  public:

	ParseXML *XML;
	int  ithSM;
	int  ithLane;
	InputParameter interface_ip;
	double clockRate,executionTime;
	double scktRatio, chip_PR_overhead, macro_PR_overhead;
	double lsq_height;
	CoreDynParam  coredynp;
	RegFUG          * rfu;
	FunctionalUnit * fpu;
	FunctionalUnit * exeu;
	interconnect * int_bypass;
	interconnect * intTagBypass;

	Component  bypass;

	EXECUG(ParseXML *XML_interface, int ithSM, int ithLane_, InputParameter* interface_ip_, double lsq_height_,const CoreDynParam & dyn_p_);
	void computeEnergy(bool is_tdp=true);
	~EXECUG();
};


class Lane :public Component {
  public:

	ParseXML *XML;
	int  ithSM;
	int  ithLane;
	InputParameter interface_ip;
	double clockRate,executionTime;
	double scktRatio, chip_PR_overhead, macro_PR_overhead;
	InstFetchUG * ifu;
	LoadStoreUG * lsu;
	EXECUG      * exu;
	SMDataCache dfilter;
	PipelinePower   * corepipe;
	CoreDynParam  coredynp;
	Lane(ParseXML *XML_interface, int ithSM, int ithLane_, InputParameter* interface_ip_, const CoreDynParam & dyn_p_);
  void update_rtparam(ParseXML *XML_interface, int ithSM_, int ithLane_);
	void set_Lane_param();
	void computeEnergy(bool is_tdp=true);
	~Lane();
};

class MemManUG :public Component {
  public:

	ParseXML *XML;
	int  ithSM;
	InputParameter interface_ip;
	CoreDynParam  coredynp;
	double clockRate,executionTime;
	double scktRatio, chip_PR_overhead, macro_PR_overhead;
	ArrayST * itlb;
	ArrayST * dtlb;

  MemManUG(ParseXML *XML_interface, int ithCore_, InputParameter* interface_ip_,const CoreDynParam & dyn_p_);
  void computeEnergy(bool is_tdp=true);
  void displayEnergy(uint32_t indent = 0,int plevel = 100, bool is_tdp=true);
  void update_rtparam(ParseXML *XML_interface, int ithSM_) { ithSM = ithSM_; };
  ~MemManUG();
};

class SM :public Component {
  public:

	ParseXML *XML;
	int  ithSM;
	InputParameter interface_ip;
	double clockRate,executionTime;
	double scktRatio, chip_PR_overhead, macro_PR_overhead;
	CoreDynParam  coredynp;


	vector<Lane *>  lanes;
	SMDataCache     dcache;
	ArrayST     *   scratchpad;
	SMInstCache       icache;
  MemManUG    *   mmu;

  Component lane;


	SM(ParseXML *XML_interface, int ithSM_, InputParameter* interface_ip_, const CoreDynParam  &dyn_p_);
  void update_rtparam(ParseXML *XML_interface, int ithSM_);
	void set_SM_param();
	void computeEnergy(bool is_tdp=true);
	~SM();
};

class GPUU: public Component {
	public:

	ParseXML *XML;
	InputParameter interface_ip;
	double clockRate,executionTime;
	double scktRatio, chip_PR_overhead, macro_PR_overhead;
	CoreDynParam  coredynp;

	vector<SM *> sms;
	SharedCache *l2array;

	GPUU(ParseXML *XML_interface, InputParameter* interface_ip_);
	void set_GPU_param();
  void update_rtparam(ParseXML *XML_interface,
    InputParameter* interface_ip_); 
	void computeEnergy(bool is_tdp=true);
	~GPUU();
};

#endif /* GPU_H_ */
