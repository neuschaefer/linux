/*
// $Id: //DTV/MP_BR/DTV_X_IDTV0801_002150_7_001_11_001/chiling/driver/linux/lib/mw/drm/divx_drm/divx_drm_40/AdpKeyStoreLib/DrmAdpKeyStore.c#1 $
// Copyright (c) 2005 DivX, Inc. http://www.divx.com/corporate
// All rights reserved.
//
// This software is the confidential and proprietary information of DivX
// Inc. ("Confidential Information").  You shall not disclose such Confidential
// Information and shall use it only in accordance with the terms of the license
// agreement you entered into with DivX, Inc.
*/

#ifndef __KERNEL__

#include "DrmAdpKeyStore.h"

#include "AdpLocalLib/DrmAdpKeyStoreLocal.h"

#include "CryptoLib/base64.h"
#include "CryptoLib/rijndael-api-fst.h"
#include "CryptoLib/Bits.h"
/* #include "CryptoLib/sha256.h" */
#include "CryptoLib/sha1.h"

#ifdef _MTK_BUILD_
#include "libc/memory.h"
#include "libc/string.h"
#include "libc/stdlib.h"
#else
#include <string.h>
#include <stdlib.h>
#endif

#else

#include "DrmAdpKeyStore.h"

#include "../AdpLocalLib/DrmAdpKeyStoreLocal.h"

#include "../CryptoLib/base64.h"
#include "../CryptoLib/rijndael-api-fst.h"
#include "../CryptoLib/Bits.h"
/* #include "../CryptoLib/sha256.h" */
#include "../CryptoLib/sha1.h"

#endif


#define TEST_RSA_KEY_ID   1

/* Replace adpBaseKeyIdTable data will localized version issued from DivX, Inc. */
static uint32_t adpBaseKeyCount = 60;
static const char *adpBaseKeyIdTable[] = 
{
	"68350122382077155371609790526077843323069757",
	"17526956348125519278333379155775791404842465",
	"07307313514643969633523034904779465997361500",
	"54733977128285238605431520951629624437529331",
	"95271872508096602223778323164547956766606317",
	"19667620404347912572341155491659903450839945",
	"58243572270469309545831683269013152299633661",
	"48838714380920672270656324638255297385378768",
	"06784853705815447409483489174064856573505938",
	"00770111886686676706976265862940452724180617",
	"53815691764967922077658113912876220915162495",
	"04312177729617650695571887594358724097271608",
	"63945301141543851075872730428987261348525394",
	"66414933091815151592855735169031788977421980",
	"02141859567992995294356684725134005814458843",
	"63206695224090071797749557616551851913902367",
	"32166867332190936760942334059749874131461526",
	"91088194239951573940293382004345521751844727",
	"92197528495960049416305937361261639677050840",
	"17330240856107841543438997133983009913947010",
	"13259619259730801548398619739515974326438404",
	"66345463236824977876404683416022743228556287",
	"91553349920876007590411937535009683049432052",
	"79786200184216186882037198523609202442582284",
	"31155661106773667020894658514427212212186554",
	"02574728702394599625389263039554984837302983",
	"47704049973879049017481401823158168936109838",
	"33181267047034345884861232414177923873921402",
	"95712790986433222039238508365438234687799024",
	"87434633147336734561911836670412892584701219",
	"46772667241777182717868628890966442544385406",
	"49035216358942073160728495530141596252425411",
	"44508642441058233617170196497655845865406903",
	"99055669180276878044689398437906169977933251",
	"03813755964652239163393926083853548089623593",
	"28939308329163842853794464938127049670250679",
	"59244270204420534452424052558003457077094806",
	"55566801756599491237873714262460883030792124",
	"83587403621836396861992555063439857575195580",
	"52547543021062104974446625289907080736360596",
	"91412179475374455877131236071755329986548065",
	"32870575011359671262202908573719122311244667",
	"37870178042902037285821789276709322633955330",
	"81623869174821026392711321648697760125077043",
	"77175978978407074183671384422084867582512807",
	"70707855331253054607736514670342511748550476",
	"30739520204616807107902904407700118418437529",
	"93237679788650327314041572175858788454649358",
	"20003680192182883691364474798010405378792219",
	"24911149782112544619931610657653650768180246",
	"64756654061975392234341568603967457924664286",
	"28369458320179089501859698186278323861884704",
	"77085984738317849374167555264822524492814026",
	"13179997570629766352239598502386239172948199",
	"93683866498185774193545671187072456445691852",
	"13371809450093784565001999346455108800080097",
	"74639151292922765147004097208050988050587443",
	"68166662441223935918083060322500394517228908",
	"39287903616748935672781132639227679536993421",
	"77658077013762464610390453611281332768339559",
};

/* Replace adpBaseKeyTable data will localized version issued from DivX, Inc. */
static const int32_t adpBaseKeyTableRows = 660;
static const char *adpBaseKeyTable[][2] = 
{
	{"0008", "6Qx7"},
	{"0019", "LyHw"},
	{"0040", "_o4H"},
	{"0058", "5uZF"},
	{"0077", "nHL_"},
	{"0097", "8w8="},
	{"0099", "buc1"},
	{"0111", "EaBP"},
	{"0113", "aFRL"},
	{"0122", "N3P2"},
	{"0137", "bcuB"},
	{"0141", "CS_O"},
	{"0178", "VLms"},
	{"0182", "xKAn"},
	{"0203", "nfZe"},
	{"0210", "31Bp"},
	{"0214", "R1yG"},
	{"0240", "y8la"},
	{"0246", "jac="},
	{"0257", "AsqJ"},
	{"0342", "LuUF"},
	{"0371", "imtT"},
	{"0381", "I8k1"},
	{"0412", "BA7e"},
	{"0415", "GVAM"},
	{"0429", "iWE3"},
	{"0431", "f0R_"},
	{"0440", "W4fG"},
	{"0470", "nh3V"},
	{"0476", "EHE="},
	{"0484", "ali-"},
	{"0496", "4toc"},
	{"0575", "56tV"},
	{"0596", "htk="},
	{"0617", "bLA="},
	{"0619", "Qrzo"},
	{"0678", "bukx"},
	{"0679", "kOw="},
	{"0695", "lc_9"},
	{"0707", "-VCQ"},
	{"0730", "G8lY"},
	{"0784", "15KV"},
	{"0807", "gnAI"},
	{"0830", "IZ4l"},
	{"0836", "ki2Q"},
	{"0840", "Cos="},
	{"0857", "zDq6"},
	{"0918", "i1QV"},
	{"0966", "rIDL"},
	{"1065", "NoAj"},
	{"1067", "EBbo"},
	{"1075", "WQrw"},
	{"1088", "aCVI"},
	{"1124", "v6Pv"},
	{"1149", "PfWd"},
	{"1184", "LPzx"},
	{"1218", "HQZV"},
	{"1219", "4rE="},
	{"1223", "QXSO"},
	{"1237", "b00m"},
	{"1254", "ogNh"},
	{"1261", "PouY"},
	{"1262", "IsT1"},
	{"1267", "5dJs"},
	{"1281", "39-V"},
	{"1282", "62JB"},
	{"1312", "PJGv"},
	{"1317", "qjC5"},
	{"1325", "5Bfw"},
	{"1337", "Tj2p"},
	{"1390", "pbWv"},
	{"1391", "1iI1"},
	{"1394", "oeGB"},
	{"1402", "UW0="},
	{"1415", "cE-F"},
	{"1426", "Tfl-"},
	{"1445", "a3Db"},
	{"1467", "5FHc"},
	{"1473", "uO2L"},
	{"1500", "Hrw="},
	{"1515", "1wWp"},
	{"1516", "K6Ss"},
	{"1522", "zQ64"},
	{"1526", "CG4="},
	{"1543", "od6W"},
	{"1544", "_dBW"},
	{"1548", "azDJ"},
	{"1592", "Pqtq"},
	{"1608", "QTg="},
	{"1618", "CSlx"},
	{"1629", "HVtE"},
	{"1659", "zmTl"},
	{"1675", "umnU"},
	{"1680", "Mk7T"},
	{"1689", "5PMp"},
	{"1699", "r4Q5"},
	{"1701", "dKE8"},
	{"1722", "vh_L"},
	{"1733", "w8np"},
	{"1748", "p3UX"},
	{"1752", "ABbM"},
	{"1755", "meSc"},
	{"1765", "_Ujv"},
	{"1784", "cFJP"},
	{"1797", "8v_h"},
	{"1802", "YY1z"},
	{"1809", "C6Lw"},
	{"1842", "hCXN"},
	{"1843", "xuBT"},
	{"1852", "4Mg="},
	{"1859", "cvRW"},
	{"1872", "gDEm"},
	{"1921", "k4U5"},
	{"1966", "563s"},
	{"1973", "ET7n"},
	{"1980", "w8g="},
	{"2000", "3wTQ"},
	{"2024", "EMYL"},
	{"2029", "qZuV"},
	{"2039", "2wqt"},
	{"2044", "_onF"},
	{"2046", "ZkiD"},
	{"2052", "j3Q="},
	{"2053", "IhuO"},
	{"2067", "_f1Q"},
	{"2077", "7aLA"},
	{"2084", "dyGX"},
	{"2095", "r58g"},
	{"2102", "3ZOM"},
	{"2122", "NTHh"},
	{"2124", "fRA="},
	{"2164", "RTdF"},
	{"2177", "FIgv"},
	{"2179", "IoQF"},
	{"2209", "s1rf"},
	{"2219", "kDI="},
	{"2223", "fEOb"},
	{"2234", "wPsv"},
	{"2240", "mMQF"},
	{"2270", "VXgV"},
	{"2276", "6Jk4"},
	{"2284", "3Yw="},
	{"2306", "uTYG"},
	{"2316", "kamt"},
	{"2346", "GUN-"},
	{"2367", "aIE="},
	{"2368", "dHK9"},
	{"2385", "884D"},
	{"2386", "gih-"},
	{"2391", "XK36"},
	{"2393", "XKPn"},
	{"2395", "7kOB"},
	{"2399", "Jr6X"},
	{"2417", "oOK-"},
	{"2418", "5mlq"},
	{"2460", "7hm_"},
	{"2463", "KGYj"},
	{"2465", "P5o="},
	{"2466", "lIN9"},
	{"2491", "ZOo0"},
	{"2495", "kiE="},
	{"2497", "-y19"},
	{"2500", "Xefk"},
	{"2507", "luUe"},
	{"2528", "MeBE"},
	{"2551", "Btc8"},
	{"2572", "SY5L"},
	{"2597", "1uVA"},
	{"2608", "Ec-w"},
	{"2613", "G8zZ"},
	{"2643", "7UNX"},
	{"2667", "wqOe"},
	{"2704", "Twyk"},
	{"2717", "Mu5m"},
	{"2790", "Ulz8"},
	{"2807", "urM="},
	{"2836", "bxGD"},
	{"2853", "rgVJ"},
	{"2855", "jSez"},
	{"2876", "XCiV"},
	{"2889", "XVlA"},
	{"2893", "erP0"},
	{"2929", "hVx9"},
	{"2933", "ZU_C"},
	{"2940", "rEw3"},
	{"2973", "cpGC"},
	{"2976", "cp-e"},
	{"2983", "mx0="},
	{"3042", "RKn-"},
	{"3059", "-D8V"},
	{"3073", "KnPQ"},
	{"3079", "mJA8"},
	{"3080", "cu4I"},
	{"3115", "lE8D"},
	{"3146", "bmtz"},
	{"3158", "HPYf"},
	{"3160", "L6K_"},
	{"3201", "g4Ok"},
	{"3216", "K_ke"},
	{"3226", "Ww2J"},
	{"3238", "8AyI"},
	{"3241", "1NSa"},
	{"3251", "FXw="},
	{"3263", "eT63"},
	{"3287", "PvJ8"},
	{"3291", "9mlS"},
	{"3299", "X-og"},
	{"3312", "eWvN"},
	{"3318", "SH3u"},
	{"3321", "7FoK"},
	{"3322", "IdWX"},
	{"3327", "Qb2U"},
	{"3333", "20lW"},
	{"3349", "tezJ"},
	{"3395", "75ZI"},
	{"3405", "uV_e"},
	{"3411", "_4BB"},
	{"3415", "0ij_"},
	{"3421", "9rE="},
	{"3434", "qQrn"},
	{"3439", "XqUx"},
	{"3481", "9U9c"},
	{"3490", "nqfE"},
	{"3516", "KEID"},
	{"3566", "8Za6"},
	{"3572", "vLBM"},
	{"3589", "dRrS"},
	{"3593", "72M="},
	{"3607", "f_uL"},
	{"3609", "Y5mi"},
	{"3610", "csot"},
	{"3617", "M6Wm"},
	{"3636", "IRye"},
	{"3639", "cnyi"},
	{"3644", "lAo9"},
	{"3661", "Qo8="},
	{"3667", "yhb4"},
	{"3673", "W3Fl"},
	{"3680", "_iXV"},
	{"3691", "qr90"},
	{"3699", "imL_"},
	{"3719", "za0P"},
	{"3730", "WCyb"},
	{"3736", "S11I"},
	{"3752", "mYS9"},
	{"3753", "R4kF"},
	{"3755", "Z2AF"},
	{"3787", "0XTu"},
	{"3809", "2c0Y"},
	{"3820", "qYZY"},
	{"3853", "OSAj"},
	{"3866", "sZUG"},
	{"3869", "gfuh"},
	{"3892", "dtQB"},
	{"3904", "uAgW"},
	{"3928", "03H9"},
	{"3939", "_iMK"},
	{"3940", "6FXD"},
	{"3945", "dY_p"},
	{"3967", "eMUp"},
	{"3977", "5nex"},
	{"3983", "nzdr"},
	{"3986", "7eb2"},
	{"4026", "11U="},
	{"4043", "E1y9"},
	{"4046", "QF1w"},
	{"4049", "E4xm"},
	{"4053", "LtES"},
	{"4064", "QQGT"},
	{"4119", "i3hi"},
	{"4177", "NlB8"},
	{"4183", "jNsH"},
	{"4193", "v16K"},
	{"4207", "83VG"},
	{"4240", "5Qi8"},
	{"4258", "G5Jg"},
	{"4270", "8aDy"},
	{"4286", "aIs="},
	{"4315", "JP9Z"},
	{"4345", "DKg3"},
	{"4358", "8xIF"},
	{"4385", "0PCA"},
	{"4389", "kEqe"},
	{"4396", "Omwm"},
	{"4410", "hPhW"},
	{"4412", "zp6q"},
	{"4425", "W6v3"},
	{"4427", "_MFN"},
	{"4438", "UFpU"},
	{"4450", "pOyw"},
	{"4452", "mEQB"},
	{"4466", "Ien9"},
	{"4500", "L6q6"},
	{"4527", "zpfj"},
	{"4547", "QkUd"},
	{"4561", "vG9P"},
	{"4564", "t5g4"},
	{"4565", "nwDY"},
	{"4569", "z4T9"},
	{"4570", "KVwn"},
	{"4579", "_F2y"},
	{"4607", "AUtZ"},
	{"4610", "JWeG"},
	{"4619", "K6ny"},
	{"4633", "PlMC"},
	{"4659", "u1ZU"},
	{"4667", "RGI="},
	{"4677", "Ge7x"},
	{"4704", "Cro="},
	{"4727", "LWg="},
	{"4728", "7A2-"},
	{"4753", "oRDw"},
	{"4770", "jkax"},
	{"4779", "Lgbm"},
	{"4791", "h7JB"},
	{"4806", "fts="},
	{"4814", "EErI"},
	{"4822", "MkPm"},
	{"4834", "V6tO"},
	{"4852", "SAIg"},
	{"4853", "VFaj"},
	{"4855", "zr_H"},
	{"4883", "DxOs"},
	{"4893", "YCz6"},
	{"4903", "5Nsa"},
	{"4933", "9F19"},
	{"4943", "kQDp"},
	{"4959", "znCw"},
	{"4974", "i4WF"},
	{"4981", "ZsMF"},
	{"5009", "T_xe"},
	{"5032", "xlZW"},
	{"5058", "zSTj"},
	{"5080", "pmtT"},
	{"5083", "6nCc"},
	{"5117", "qrar"},
	{"5134", "Zb7X"},
	{"5146", "NHGy"},
	{"5147", "33EU"},
	{"5157", "5Udm"},
	{"5184", "ixhp"},
	{"5216", "vT6W"},
	{"5217", "xaDj"},
	{"5223", "kfpU"},
	{"5230", "mz3y"},
	{"5242", "yKrv"},
	{"5244", "tUNd"},
	{"5254", "KDEI"},
	{"5255", "cSEd"},
	{"5294", "NkYs"},
	{"5301", "NmgT"},
	{"5305", "0bRr"},
	{"5330", "3IU="},
	{"5361", "1RBf"},
	{"5371", "Yg73"},
	{"5381", "IMao"},
	{"5394", "XOg="},
	{"5406", "TZc="},
	{"5411", "vA0="},
	{"5438", "GK-E"},
	{"5456", "CthU"},
	{"5463", "0AlU"},
	{"5464", "O5uL"},
	{"5473", "aRqz"},
	{"5480", "-MzW"},
	{"5506", "4olt"},
	{"5526", "RSyZ"},
	{"5549", "D9d7"},
	{"5556", "CDrT"},
	{"5580", "NpI="},
	{"5661", "cOHT"},
	{"5669", "9iOL"},
	{"5672", "16Ft"},
	{"5679", "1E0B"},
	{"5691", "WheL"},
	{"5706", "fMMf"},
	{"5718", "pa9k"},
	{"5761", "Ac8G"},
	{"5775", "W5pM"},
	{"5823", "nuVM"},
	{"5824", "zCTm"},
	{"5851", "4cxy"},
	{"5858", "WfLZ"},
	{"5877", "7VDZ"},
	{"5884", "O1BO"},
	{"5918", "UYmG"},
	{"5924", "-kZx"},
	{"5938", "A6A="},
	{"5962", "CBPw"},
	{"5967", "K157"},
	{"5978", "RMky"},
	{"5984", "fJ-o"},
	{"6004", "MVNN"},
	{"6022", "vbSI"},
	{"6032", "CNq8"},
	{"6077", "tK97"},
	{"6097", "pdbV"},
	{"6167", "Wz03"},
	{"6188", "s-hF"},
	{"6200", "MLUM"},
	{"6210", "lCl9"},
	{"6218", "98SV"},
	{"6244", "uzaF"},
	{"6246", "eIjk"},
	{"6278", "DeXP"},
	{"6287", "MgQ="},
	{"6303", "UBpY"},
	{"6317", "wKw="},
	{"6320", "et5S"},
	{"6352", "441R"},
	{"6384", "t4EJ"},
	{"6392", "I1sb"},
	{"6394", "ARV4"},
	{"6396", "XEG7"},
	{"6455", "qqQ6"},
	{"6475", "HmZU"},
	{"6493", "D-la"},
	{"6507", "oQYp"},
	{"6540", "8yMa"},
	{"6551", "hQrq"},
	{"6554", "CzA="},
	{"6563", "gA1g"},
	{"6581", "wYXN"},
	{"6586", "Tunx"},
	{"6634", "NHWo"},
	{"6641", "NcNm"},
	{"6654", "RG8u"},
	{"6660", "GKrg"},
	{"6662", "JpWA"},
	{"6695", "MRrA"},
	{"6706", "h4Sn"},
	{"6709", "E7ER"},
	{"6713", "VRCx"},
	{"6760", "jhua"},
	{"6792", "Dv7T"},
	{"6795", "oml6"},
	{"6801", "_abP"},
	{"6816", "2p-A"},
	{"6818", "fjWN"},
	{"6830", "YkBX"},
	{"6833", "Z2lu"},
	{"6835", "e58P"},
	{"6860", "cHr6"},
	{"6861", "ouHP"},
	{"6867", "ybH1"},
	{"6882", "Q-gP"},
	{"6893", "W9qI"},
	{"6903", "De4="},
	{"6930", "8kX6"},
	{"6956", "0nqv"},
	{"7010", "2lQ="},
	{"7020", "jlu5"},
	{"7023", "-gMl"},
	{"7025", "fINe"},
	{"7043", "Gi4="},
	{"7058", "T5rJ"},
	{"7070", "4mgT"},
	{"7072", "C6-h"},
	{"7107", "7bdK"},
	{"7113", "Wta8"},
	{"7118", "MveU"},
	{"7217", "1K2H"},
	{"7240", "9VEF"},
	{"7284", "UIRd"},
	{"7285", "scw_"},
	{"7294", "-Y19"},
	{"7296", "rT2W"},
	{"7313", "A7DX"},
	{"7314", "Qrn2"},
	{"7350", "u0Hx"},
	{"7365", "NZ8P"},
	{"7366", "j8zs"},
	{"7383", "Qfls"},
	{"7392", "miJ1"},
	{"7403", "0PdF"},
	{"7409", "2U-T"},
	{"7432", "tRae"},
	{"7443", "S5Q="},
	{"7445", "r4qZ"},
	{"7463", "_CpU"},
	{"7479", "-JYn"},
	{"7495", "0qMc"},
	{"7519", "pr4t"},
	{"7528", "1E04"},
	{"7529", "Gbw="},
	{"7539", "YqMl"},
	{"7543", "1wOy"},
	{"7565", "rbh3"},
	{"7590", "fIMy"},
	{"7600", "-4Xf"},
	{"7601", "thOr"},
	{"7620", "W4us"},
	{"7649", "mar0"},
	{"7653", "yIeC"},
	{"7655", "ka2d"},
	{"7679", "0xEy"},
	{"7687", "DT4a"},
	{"7700", "ERO6"},
	{"7705", "2jPI"},
	{"7708", "KPxa"},
	{"7709", "NLFp"},
	{"7715", "0hDY"},
	{"7717", "v9ye"},
	{"7718", "Ja-M"},
	{"7742", "Aihk"},
	{"7765", "KTTX"},
	{"7783", "z3Q0"},
	{"7793", "lFyO"},
	{"7811", "Kkgp"},
	{"7821", "p8yc"},
	{"7855", "9HzB"},
	{"7876", "gAZ-"},
	{"7879", "NpJb"},
	{"7884", "E0yV"},
	{"7886", "KMzI"},
	{"7889", "vwzA"},
	{"7903", "Z25n"},
	{"7904", "MV3g"},
	{"7906", "NSrq"},
	{"7908", "AnFR"},
	{"7914", "dckX"},
	{"7915", "nYfP"},
	{"7944", "AjDq"},
	{"7978", "HYjg"},
	{"8003", "b7TB"},
	{"8010", "jm4a"},
	{"8044", "1kzZ"},
	{"8050", "B4Fy"},
	{"8065", "pSI="},
	{"8077", "f_QC"},
	{"8127", "2CI_"},
	{"8162", "3Bi0"},
	{"8194", "bMY7"},
	{"8199", "Fc8="},
	{"8200", "-UzK"},
	{"8217", "5VoI"},
	{"8251", "6b3j"},
	{"8255", "Ls7i"},
	{"8288", "pql2"},
	{"8316", "3iQC"},
	{"8326", "Kjq7"},
	{"8341", "uZdf"},
	{"8358", "8elL"},
	{"8404", "l4g="},
	{"8433", "WbUg"},
	{"8442", "f0cY"},
	{"8458", "aF0R"},
	{"8470", "xobM"},
	{"8472", "pCXJ"},
	{"8519", "kHEk"},
	{"8523", "OTuQ"},
	{"8537", "asHA"},
	{"8557", "6aLQ"},
	{"8561", "bD_Z"},
	{"8565", "n3ke"},
	{"8575", "SVUq"},
	{"8577", "SzTf"},
	{"8596", "ul-t"},
	{"8605", "nMy3"},
	{"8612", "SX7D"},
	{"8642", "8mR-"},
	{"8654", "p4VU"},
	{"8667", "FgGx"},
	{"8675", "p_nd"},
	{"8686", "ZCWl"},
	{"8697", "D-Lr"},
	{"8714", "nMdb"},
	{"8727", "aOrI"},
	{"8737", "W2j3"},
	{"8741", "sqny"},
	{"8743", "Z16n"},
	{"8759", "xHSB"},
	{"8768", "FFU="},
	{"8779", "xJvF"},
	{"8830", "bD-x"},
	{"8843", "MlM="},
	{"8866", "xVNw"},
	{"8908", "HVM="},
	{"8917", "Pzh0"},
	{"8925", "Z1IQ"},
	{"8927", "TrsV"},
	{"8946", "Qpwu"},
	{"8962", "kDVn"},
	{"8987", "CFmF"},
	{"9007", "DKMr"},
	{"9013", "VfVs"},
	{"9017", "a6US"},
	{"9024", "0Fc="},
	{"9029", "D-W1"},
	{"9031", "lQbi"},
	{"9034", "s7Wm"},
	{"9052", "qg5O"},
	{"9093", "hLF0"},
	{"9108", "CqES"},
	{"9118", "vuzf"},
	{"9141", "gRh9"},
	{"9151", "oTBV"},
	{"9155", "ovkk"},
	{"9163", "1kAt"},
	{"9208", "m8IO"},
	{"9219", "ktrj"},
	{"9227", "UFXs"},
	{"9238", "SAMF"},
	{"9278", "KxXm"},
	{"9281", "Ca36"},
	{"9299", "LC-V"},
	{"9308", "pjas"},
	{"9316", "0ST4"},
	{"9323", "1XS3"},
	{"9331", "r0s="},
	{"9358", "gfs="},
	{"9368", "CS9N"},
	{"9374", "B-tJ"},
	{"9378", "cjJL"},
	{"9416", "Pm32"},
	{"9423", "mg-Q"},
	{"9458", "qbjU"},
	{"9459", "NvA3"},
	{"9501", "aBLr"},
	{"9515", "22FP"},
	{"9520", "81VH"},
	{"9527", "tlAJ"},
	{"9545", "x1da"},
	{"9553", "kQec"},
	{"9554", "J1L_"},
	{"9559", "z7c="},
	{"9567", "RqgE"},
	{"9571", "OmfI"},
	{"9619", "5VWQ"},
	{"9625", "oYlm"},
	{"9633", "RhsG"},
	{"9646", "QNTy"},
	{"9649", "ZaiV"},
	{"9660", "_hZq"},
	{"9713", "xRUY"},
	{"9720", "wOzJ"},
	{"9727", "MkG8"},
	{"9736", "4INy"},
	{"9738", "cxCc"},
	{"9743", "DB0E"},
	{"9749", "CtZA"},
	{"9757", "hIo="},
	{"9762", "0TA4"},
	{"9784", "iQXa"},
	{"9818", "_Qxw"},
	{"9838", "k1U="},
	{"9843", "aMvK"},
	{"9848", "zmhO"},
	{"9850", "57bT"},
	{"9852", "ZpWY"},
	{"9864", "_t_U"},
	{"9880", "aQys"},
	{"9905", "xpc5"},
	{"9907", "CT3a"},
	{"9925", "tNWn"},
	{"9934", "CvoJ"},
	{"9945", "Tjw="},
	{"9949", "6_JE"},
	{"9963", "UMv8"},
	{"9997", "t0Lk"},
};

static uint8_t messageKeyTable[MESSAGE_KEY_INDEX_TOTAL][ADP_MESSAGE_KEY_SIZE_BYTES] =
{
	{ 0xA1, 0xBD, 0x49, 0x71, 0xF8, 0x59, 0xCE, 0xFD, 0xDF, 0x9C, 0xAD, 0xE9, 0xA8, 0xE1, 0x16, 0xE4, 0x8E, 0x43, 0x9E, 0xA6 },
	{ 0x48, 0x34, 0x9B, 0x3A, 0x82, 0x42, 0x14, 0x7F, 0xB0, 0x98, 0xF6, 0x08, 0x9A, 0x64, 0x5A, 0x9E, 0xAC, 0xD7, 0x12, 0x65 },
	{ 0xDE, 0xC4, 0xE3, 0x8A, 0x72, 0x65, 0x72, 0x57, 0x28, 0x20, 0x70, 0x0A, 0x32, 0xCD, 0xF4, 0x5E, 0x5E, 0xA0, 0x1A, 0xDF },
	{ 0xEA, 0x17, 0x5D, 0xDC, 0x0C, 0x57, 0xF7, 0x8A, 0x4F, 0x5D, 0x28, 0x4E, 0x68, 0x07, 0xFD, 0x28, 0xBA, 0x58, 0x7C, 0x8C },
	{ 0x2C, 0xE1, 0x13, 0x3D, 0xD9, 0x77, 0xD9, 0x49, 0xB4, 0xFC, 0x9A, 0xDF, 0xBE, 0xBB, 0xE8, 0xB3, 0xE9, 0x66, 0xFB, 0xEC },
	{ 0xBA, 0xC1, 0x85, 0xFB, 0xA2, 0xFD, 0x75, 0x8F, 0xA4, 0x23, 0xB6, 0x14, 0x7D, 0xCB, 0xA5, 0xBB, 0x27, 0xBF, 0xAF, 0xA5 },
	{ 0x5B, 0x58, 0xC9, 0x17, 0xAA, 0xD6, 0x71, 0x42, 0xF6, 0xEB, 0xE5, 0x95, 0xB0, 0x03, 0x3C, 0xCB, 0x2C, 0x34, 0xF4, 0xF8 },
	{ 0xBE, 0x95, 0x81, 0x4E, 0x81, 0xDA, 0x57, 0x7C, 0xBC, 0x87, 0xCC, 0x0E, 0x99, 0x68, 0x75, 0xF9, 0x6E, 0x46, 0x15, 0x46 },
	{ 0xB8, 0xBC, 0x20, 0xB5, 0xD4, 0x00, 0xD3, 0x37, 0x1C, 0x93, 0x46, 0x00, 0x44, 0x18, 0x70, 0xCF, 0x99, 0x5F, 0xB7, 0xE3 },
	{ 0x8E, 0x1F, 0x70, 0xC8, 0x06, 0x43, 0x19, 0xE5, 0x66, 0xD6, 0x3B, 0x36, 0x4F, 0xCD, 0xCD, 0xC6, 0xA9, 0x8E, 0x47, 0xD8 },
	{ 0x45, 0x2F, 0xA0, 0x4B, 0x30, 0xAB, 0xDB, 0x09, 0x63, 0x67, 0xBD, 0xAB, 0x34, 0xEC, 0x04, 0x00, 0xB1, 0xCB, 0xEA, 0xBE },
	{ 0xDF, 0xED, 0xB5, 0xC7, 0x9C, 0x7F, 0x82, 0x1C, 0x3F, 0xC8, 0x5B, 0x22, 0x34, 0x79, 0xFD, 0xC2, 0x10, 0x42, 0x16, 0x9A },
	{ 0xFD, 0xA3, 0x49, 0x81, 0x95, 0x30, 0x87, 0x01, 0x80, 0x6D, 0xCA, 0xFD, 0x68, 0x7E, 0x16, 0x23, 0xBF, 0x87, 0x56, 0xA6 },
	{ 0xEB, 0x51, 0x5F, 0x6A, 0x06, 0xFD, 0x19, 0x62, 0x96, 0xCD, 0xED, 0xB8, 0xD8, 0xC6, 0x86, 0x86, 0xD9, 0x29, 0x0D, 0x34 },
	{ 0x4E, 0xA4, 0x90, 0x9A, 0x83, 0x42, 0x14, 0x7F, 0xB0, 0x98, 0xF6, 0x08, 0x9A, 0x64, 0x5A, 0x9E, 0xAC, 0xD7, 0x12, 0x65 },
	{ 0x09, 0xBC, 0x09, 0x7E, 0xA8, 0x59, 0xCE, 0xFD, 0xDF, 0x9C, 0xAD, 0xE9, 0xA8, 0xE1, 0x16, 0xE4, 0x8E, 0x43, 0x9E, 0xA6 }
};


static uint8_t getLocalizedAdpBaseKey(uint8_t *localBaseKey, uint8_t *trueBaseKey);

static uint8_t getLocalizedAdpBaseKey(uint8_t *localBaseKey, uint8_t *trueBaseKey)
{
	uint8_t hardwareSecret[ADP_HARDWARE_SECRET_BYTES] = { 0 };
	uint8_t hardwareKey[ADP_HARDWARE_KEY_BYTES] = { 0 };
	char hardwareKeyHex[(ADP_HARDWARE_KEY_BYTES * HEX_OCTET_PER_BYTE) + 1];
	uint8_t ciphered[ADP_BASE_KEY_SIZE_BYTES] = { 0 };
/*	char hardwareSecretHash[SHA256_BYTES];*/
	char *hardwareSecretHash;
	unsigned int outlength;
	keyInstance hardwareKeyInstance;
	int32_t decryptedSize;
	cipherInstance cipher;
	uint8_t result = ERROR_KEYSTORE_NONE;
	int32_t cryptoResult = TRUE;

	localGetHardwareSecret(hardwareSecret);

/*	sha256(hardwareSecret, sizeof(hardwareSecret), (uint8_t *)hardwareSecretHash);

	memcpy(hardwareKey, hardwareSecretHash, ADP_HARDWARE_KEY_BYTES);
*/
	hardwareSecretHash = sha1GetHash(hardwareSecret, 32, &outlength);

	memcpy(hardwareKey, hardwareSecretHash, ADP_HARDWARE_KEY_BYTES);

	convertBinToHex(hardwareKey, ADP_HARDWARE_KEY_BYTES, hardwareKeyHex);
	memcpy(ciphered, localBaseKey, ADP_BASE_KEY_SIZE_BYTES);

	cryptoResult = cipherInit(&cipher, MODE_ECB, NULL);
	if(TRUE == cryptoResult)
	{
		cryptoResult = makeKey(&hardwareKeyInstance, DIR_DECRYPT, ADP_HARDWARE_KEY_BITS, hardwareKeyHex);
		if (TRUE == cryptoResult)
		{
			decryptedSize = blockDecrypt(&cipher, &hardwareKeyInstance, ciphered, ADP_BASE_KEY_SIZE_BITS, trueBaseKey);
			if (ADP_BASE_KEY_SIZE_BITS != decryptedSize)
			{
				result = ERROR_KEYSTORE_DECRYPTION_FAILED;
			}
		}
		else
		{
			result = ERROR_KEYSTORE_DECRYPTION_FAILED;
		}
	}
	else
	{
		result = ERROR_KEYSTORE_DECRYPTION_FAILED;
	}

	return result;
}

uint8_t getAdpBaseKeyFromId(const uint8_t *id, uint8_t *adpBaseKey)
{
	uint8_t result = ERROR_KEYSTORE_NONE;

	/* The AES portable keys for a 32 byte key are 44 bytes long.
	 The portable key is divided into elevent four byte pieces; 
	 each piece has a random number id. */
	int32_t j = 0;
	int32_t k = 0;
	int32_t keyTableOffset = -1;
	char portableKey[ADP_BASE_KEY_ID_SIZE_BYTES] = {0};
	char *adpBaseKeyAllocated;
	uint32_t keySize = 0;

	for (j = 0; j < 11; j++)
	{
		int firstChar = j * 4;
		char idPart[4];

		strncpy(idPart, (const char *)(id + firstChar), 4);

		for(k = 0; k < adpBaseKeyTableRows; k++)
		{
			if(0 == strncmp(idPart, adpBaseKeyTable[k][0], 4))
			{
				keyTableOffset = k;
				memcpy(portableKey + firstChar, adpBaseKeyTable[k][1], 4);
				break;
			}
		}

		if(keyTableOffset == -1)
		{
			return ERROR_KEYSTORE_INVALID_BASE_KEY;
		}
	}

	adpBaseKeyAllocated = base64Decode(portableKey, ADP_BASE_KEY_ID_SIZE_BYTES, &keySize);  /* This mallocs result. */
	if (keySize != ADP_BASE_KEY_SIZE_BYTES)
	{
		result = ERROR_KEYSTORE_INVALID_BASE_KEY;
	}

	result = getLocalizedAdpBaseKey((uint8_t *)adpBaseKeyAllocated, adpBaseKey);
	free(adpBaseKeyAllocated);
	
	return result;
}

uint8_t getAdpBaseHexKeyFromId(const uint8_t *baseKeyId, char *adpBaseHexKey)
{
	uint8_t result = ERROR_KEYSTORE_NONE;
	uint8_t adpBaseKey[ADP_BASE_KEY_SIZE_BYTES] = {0};

	result = getAdpBaseKeyFromId(baseKeyId, adpBaseKey);
	if (ERROR_KEYSTORE_NONE == result)
	{
		convertBinToHex(adpBaseKey, ADP_BASE_KEY_SIZE_BYTES, adpBaseHexKey);
	}

	return result;
}

uint8_t getAdpBaseKeyIdFromIndex(uint8_t index, uint8_t *baseKeyId)
{
	uint8_t result = ERROR_KEYSTORE_INVALID_BASE_KEY;

	if (!(index > (adpBaseKeyCount - 1)))
	{
		memcpy(baseKeyId, adpBaseKeyIdTable[index], ADP_BASE_KEY_ID_SIZE_BYTES);
		result = ERROR_KEYSTORE_NONE;
	}

	return result;
}

uint8_t getProtectionFormat(uint8_t *protectionFormatFlag)
{
	uint8_t result = ERROR_KEYSTORE_MEMORY;

	if (NULL != protectionFormatFlag)
	{
		*protectionFormatFlag = 1;  /* This signals to server to sign the activation. */
		result = ERROR_KEYSTORE_NONE;
	}

	return result;
}

uint8_t getMessageKey(uint8_t protectionFormatFlag, uint8_t *messageKey)
{
	uint8_t result = ERROR_KEYSTORE_ID_NOT_FOUND;

	if (!(protectionFormatFlag > (MESSAGE_KEY_INDEX_TOTAL - 1)))
	{
		memcpy(messageKey, messageKeyTable[protectionFormatFlag], ADP_MESSAGE_KEY_SIZE_BYTES);
		result = ERROR_KEYSTORE_NONE;
	}

	return result;
}

#ifdef SUPPORT_SIGNED_ACTIVATION
uint8_t getPublicKeyFromId(const uint32_t id,
						   char nHex[ADP_RSA_HEX_KEY_SIZE_BYTES],
						   char eHex[ADP_RSA_HEX_E_EXP_SIZE_BYTES])
{
	uint8_t result = ERROR_KEYSTORE_ID_NOT_FOUND;

	result = localGetPublicKeyFromId(id, nHex, eHex);

	return result;
}
#endif
