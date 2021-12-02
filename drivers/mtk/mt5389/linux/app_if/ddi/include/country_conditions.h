/******************************************************************************
 *	 DTV LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 *	 Copyright(c) 2008 by LG Electronics Inc.
 *
 *	 All rights reserved. No part of this work may be reproduced, stored in a
 *	 retrieval system, or transmitted by any means without prior written
 *	 permission of LG Electronics Inc.
 *****************************************************************************/

/** @file country_conditions.h
 *
 * This header file defines the macros defining country conditions.
 *
 *	@author 	David (DONG-IL) Lee
 *	@version	1.0
 *	@date		2008.05.08
 *	@note		This file shall be included only in country.h
 *	@see		ISO 3166-1
 */

#ifndef WIN32_SIM
#ifndef WIN32_SIM
#ifndef _COUNTRY_H_
	#error	NOTE: This file should be included only in country.h
#endif
#endif
#endif
#ifndef	_COUNTRY_CONDITIONS_H_
#define	_COUNTRY_CONDITIONS_H_
#ifdef __cplusplus
extern "C" {
#endif

#define	RMM_COUNTRY_IS_AF	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_AF)	// country == Afghanistan
#define	RMM_COUNTRY_IS_AX	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_AX)	// country == Aland Islands
#define	RMM_COUNTRY_IS_AL	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_AL)	// country == Albania
#define	RMM_COUNTRY_IS_DZ	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_DZ)	// country == Algeria
#define	RMM_COUNTRY_IS_AS	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_AS)	// country == American Samoa
#define	RMM_COUNTRY_IS_AD	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_AD)	// country == Andorra
#define	RMM_COUNTRY_IS_AO	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_AO)	// country == Angola
#define	RMM_COUNTRY_IS_AI	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_AI)	// country == Anguilla
#define	RMM_COUNTRY_IS_AQ	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_AQ)	// country == Antarctica
#define	RMM_COUNTRY_IS_AG	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_AG)	// country == Antigua and Barbuda
#define	RMM_COUNTRY_IS_AR	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_AR)	// country == Argentina
#define	RMM_COUNTRY_IS_AM	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_AM)	// country == Armenia
#define	RMM_COUNTRY_IS_AW	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_AW)	// country == Aruba
#define	RMM_COUNTRY_IS_AU	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_AU)	// country == Australia
#define	RMM_COUNTRY_IS_AT	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_AT)	// country == Austria
#define	RMM_COUNTRY_IS_AZ	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_AZ)	// country == Azerbaijan

#define	RMM_COUNTRY_IS_BS	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_BS)	// country == Bahamas
#define	RMM_COUNTRY_IS_BH	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_BH)	// country == Bahrain
#define	RMM_COUNTRY_IS_BD	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_BD)	// country == Bangladesh
#define	RMM_COUNTRY_IS_BB	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_BB)	// country == Barbados
#define	RMM_COUNTRY_IS_BY	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_BY)	// country == Belarus
#define	RMM_COUNTRY_IS_BE	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_BE)	// country == Belgium
#define	RMM_COUNTRY_IS_BZ	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_BZ)	// country == Belize
#define	RMM_COUNTRY_IS_BJ	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_BJ)	// country == Benin
#define	RMM_COUNTRY_IS_BM	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_BM)	// country == Bermuda
#define	RMM_COUNTRY_IS_BT	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_BT)	// country == Bhutan
#define	RMM_COUNTRY_IS_BO	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_BO)	// country == Bolivia
#define	RMM_COUNTRY_IS_BA	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_BA)	// country == Bosnia and Herzegovina
#define	RMM_COUNTRY_IS_BW	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_BW)	// country == Botswana
#define	RMM_COUNTRY_IS_BV	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_BV)	// country == Bouvet Island
#define	RMM_COUNTRY_IS_BR	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_BR)	// country == Brazil
#define	RMM_COUNTRY_IS_IO	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_IO)	// country == British Indian Ocean Territory
#define	RMM_COUNTRY_IS_BN	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_BN)	// country == Brunei Darussalam
#define	RMM_COUNTRY_IS_BG	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_BG)	// country == Bulgaria
#define	RMM_COUNTRY_IS_BF	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_BF)	// country == Burkina Faso
#define	RMM_COUNTRY_IS_BI	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_BI)	// country == Burundi

#define	RMM_COUNTRY_IS_KH	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_KH)	// country == Cambodia
#define	RMM_COUNTRY_IS_CM	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_CM)	// country == Cameroon
#define	RMM_COUNTRY_IS_CA	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_CA)	// country == Canada
#define	RMM_COUNTRY_IS_CV	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_CV)	// country == Cape Verde
#define	RMM_COUNTRY_IS_KY	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_KY)	// country == Cayman Islands
#define	RMM_COUNTRY_IS_CF	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_CF)	// country == Central African Republic
#define	RMM_COUNTRY_IS_TD	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_TD)	// country == Chad
#define	RMM_COUNTRY_IS_CL	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_CL)	// country == Chile
//#define RMM_COUNRTY_IS_CN => moved to china-city option
#define	RMM_COUNTRY_IS_CX	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_CX)	// country == Christmas Island
#define	RMM_COUNTRY_IS_CC	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_CC)	// country == Cocos (Keeling) Islands
#define	RMM_COUNTRY_IS_CO	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_CO)	// country == Colombia
#define	RMM_COUNTRY_IS_KM	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_KM)	// country == Comoros
#define	RMM_COUNTRY_IS_CG	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_CG)	// country == Congo
#define	RMM_COUNTRY_IS_CD	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_CD)	// country == Congo, Democratic Republic of the
#define	RMM_COUNTRY_IS_CK	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_CK)	// country == Cook Islands
#define	RMM_COUNTRY_IS_CR	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_CR)	// country == Costa Rica
#define	RMM_COUNTRY_IS_CI	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_CI)	// country == Cote d'Ivoire
#define	RMM_COUNTRY_IS_HR	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_HR)	// country == Croatia
#define	RMM_COUNTRY_IS_CU	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_CU)	// country == Cuba
#define	RMM_COUNTRY_IS_CY	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_CY)	// country == Cyprus
#define	RMM_COUNTRY_IS_CZ	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_CZ)	// country == Czech Republic

#define	RMM_COUNTRY_IS_DK	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_DK)	// country == Denmark
#define	RMM_COUNTRY_IS_DJ	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_DJ)	// country == Djibouti
#define	RMM_COUNTRY_IS_DM	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_DM)	// country == Dominica
#define	RMM_COUNTRY_IS_DO	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_DO)	// country == Dominican Republic

#define	RMM_COUNTRY_IS_EC	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_EC)	// country == Ecuador
#define	RMM_COUNTRY_IS_EG	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_EG)	// country == Egypt
#define	RMM_COUNTRY_IS_SV	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_SV)	// country == El Salvador
#define	RMM_COUNTRY_IS_GQ	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_GQ)	// country == Equatorial Guinea
#define	RMM_COUNTRY_IS_ER	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_ER)	// country == Eritrea
#define	RMM_COUNTRY_IS_EE	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_EE)	// country == Estonia
#define	RMM_COUNTRY_IS_ET	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_ET)	// country == Ethiopia

#define	RMM_COUNTRY_IS_FK	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_FK)	// country == Falkland Islands (Malvinas)
#define	RMM_COUNTRY_IS_FO	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_FO)	// country == Faroe Islands
#define	RMM_COUNTRY_IS_FJ	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_FJ)	// country == Fiji
#define	RMM_COUNTRY_IS_FI	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_FI)	// country == Finland
#define	RMM_COUNTRY_IS_FR	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_FR)	// country == France
#define	RMM_COUNTRY_IS_GF	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_GF)	// country == French Guiana
#define	RMM_COUNTRY_IS_PF	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_PF)	// country == French Polynesia
#define	RMM_COUNTRY_IS_TF	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_TF)	// country == French Southern Territories

#define	RMM_COUNTRY_IS_GA	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_GA)	// country == Gabon
#define	RMM_COUNTRY_IS_GM	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_GM)	// country == Gambia
#define	RMM_COUNTRY_IS_GE	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_GE)	// country == Georgia
#define	RMM_COUNTRY_IS_DE	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_DE)	// country == Germany
#define	RMM_COUNTRY_IS_GH	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_GH)	// country == Ghana
#define	RMM_COUNTRY_IS_GI	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_GI)	// country == Gibraltar
#define	RMM_COUNTRY_IS_GR	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_GR)	// country == Greece
#define	RMM_COUNTRY_IS_GL	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_GL)	// country == Greenland
#define	RMM_COUNTRY_IS_GD	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_GD)	// country == Grenada
#define	RMM_COUNTRY_IS_GP	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_GP)	// country == Guadeloupe
#define	RMM_COUNTRY_IS_GU	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_GU)	// country == Guam
#define	RMM_COUNTRY_IS_GT	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_GT)	// country == Guatemala
#define	RMM_COUNTRY_IS_GG	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_GG)	// country == Guernsey
#define	RMM_COUNTRY_IS_GN	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_GN)	// country == Guinea
#define	RMM_COUNTRY_IS_GW	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_GW)	// country == Guinea-Bissau
#define	RMM_COUNTRY_IS_GY	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_GY)	// country == Guyana

#define	RMM_COUNTRY_IS_HT	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_HT)	// country == Haiti
#define	RMM_COUNTRY_IS_HM	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_HM)	// country == Heard Island and McDonald Islands
#define	RMM_COUNTRY_IS_VA	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_VA)	// country == Holy See (Vatican City State)
#define	RMM_COUNTRY_IS_HN	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_HN)	// country == Honduras
#define	RMM_COUNTRY_IS_HK	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_HK)	// country == Hong Kong
#define	RMM_COUNTRY_IS_HU	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_HU)	// country == Hungary

#define	RMM_COUNTRY_IS_IS	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_IS)	// country == Iceland
#define	RMM_COUNTRY_IS_IN	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_IN)	// country == India
#define	RMM_COUNTRY_IS_ID	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_ID)	// country == Indonesia
#define	RMM_COUNTRY_IS_IR	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_IR)	// country == Iran, Islamic Republic of
#define	RMM_COUNTRY_IS_IQ	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_IQ)	// country == Iraq
#define	RMM_COUNTRY_IS_IE	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_IE)	// country == Ireland
#define	RMM_COUNTRY_IS_IM	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_IM)	// country == Isle of Man
#define	RMM_COUNTRY_IS_IL	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_IL)	// country == Israel
#define	RMM_COUNTRY_IS_IT	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_IT)	// country == Italy

#define	RMM_COUNTRY_IS_JM	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_JM)	// country == Jamaica
#define	RMM_COUNTRY_IS_JP	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_JP)	// country == Japan
#define	RMM_COUNTRY_IS_JE	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_JE)	// country == Jersey
#define	RMM_COUNTRY_IS_JO	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_JO)	// country == Jordan

#define	RMM_COUNTRY_IS_KZ	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_KZ)	// country == Kazakhstan
#define	RMM_COUNTRY_IS_KE	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_KE)	// country == Kenya
#define	RMM_COUNTRY_IS_KI	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_KI)	// country == Kiribati
#define	RMM_COUNTRY_IS_KP	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_KP)	// country == Korea, Democratic People's Republic of
#define	RMM_COUNTRY_IS_KR	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_KR)	// country == Korea, Republic of
#define	RMM_COUNTRY_IS_KW	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_KW)	// country == Kuwait
#define	RMM_COUNTRY_IS_KG	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_KG)	// country == Kyrgyzstan

#define	RMM_COUNTRY_IS_LA	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_LA)	// country == Lao People's Democratic Republic
#define	RMM_COUNTRY_IS_LV	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_LV)	// country == Latvia
#define	RMM_COUNTRY_IS_LB	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_LB)	// country == Lebanon
#define	RMM_COUNTRY_IS_LS	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_LS)	// country == Lesotho
#define	RMM_COUNTRY_IS_LR	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_LR)	// country == Liberia
#define	RMM_COUNTRY_IS_LY	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_LY)	// country == Libyan Arab Jamahiriya
#define	RMM_COUNTRY_IS_LI	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_LI)	// country == Liechtenstein
#define	RMM_COUNTRY_IS_LT	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_LT)	// country == Lithuania
#define	RMM_COUNTRY_IS_LU	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_LU)	// country == Luxembourg

#define	RMM_COUNTRY_IS_MO	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_MO)	// country == Macao
#define	RMM_COUNTRY_IS_MK	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_MK)	// country == Macedonia, the former Yugoslav Republic of
#define	RMM_COUNTRY_IS_MG	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_MG)	// country == Madagascar
#define	RMM_COUNTRY_IS_MW	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_MW)	// country == Malawi
#define	RMM_COUNTRY_IS_MY	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_MY)	// country == Malaysia
#define	RMM_COUNTRY_IS_MV	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_MV)	// country == Maldives
#define	RMM_COUNTRY_IS_ML	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_ML)	// country == Mali
#define	RMM_COUNTRY_IS_MT	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_MT)	// country == Malta
#define	RMM_COUNTRY_IS_MH	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_MH)	// country == Marshall Islands
#define	RMM_COUNTRY_IS_MQ	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_MQ)	// country == Martinique
#define	RMM_COUNTRY_IS_MR	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_MR)	// country == Mauritania
#define	RMM_COUNTRY_IS_MU	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_MU)	// country == Mauritius
#define	RMM_COUNTRY_IS_YT	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_YT)	// country == Mayotte
#define	RMM_COUNTRY_IS_MX	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_MX)	// country == Mexico
#define	RMM_COUNTRY_IS_FM	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_FM)	// country == Micronesia, Federated States of
#define	RMM_COUNTRY_IS_MD	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_MD)	// country == Moldova
#define	RMM_COUNTRY_IS_MC	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_MC)	// country == Monaco
#define	RMM_COUNTRY_IS_MN	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_MN)	// country == Mongolia
#define	RMM_COUNTRY_IS_ME	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_ME)	// country == Montenegro
#define	RMM_COUNTRY_IS_MS	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_MS)	// country == Montserrat
#define	RMM_COUNTRY_IS_MA	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_MA)	// country == Morocco
#define	RMM_COUNTRY_IS_MZ	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_MZ)	// country == Mozambique
#define	RMM_COUNTRY_IS_MM	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_MM)	// country == Myanmar

#define	RMM_COUNTRY_IS_NA	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_NA)	// country == Namibia
#define	RMM_COUNTRY_IS_NR	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_NR)	// country == Nauru
#define	RMM_COUNTRY_IS_NP	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_NP)	// country == Nepal
#define	RMM_COUNTRY_IS_NL	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_NL)	// country == Netherlands
#define	RMM_COUNTRY_IS_AN	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_AN)	// country == Netherlands Antilles
#define	RMM_COUNTRY_IS_NC	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_NC)	// country == New Caledonia
#define	RMM_COUNTRY_IS_NZ	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_NZ)	// country == New Zealand
#define	RMM_COUNTRY_IS_NI	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_NI)	// country == Nicaragua
#define	RMM_COUNTRY_IS_NE	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_NE)	// country == Niger
#define	RMM_COUNTRY_IS_NG	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_NG)	// country == Nigeria
#define	RMM_COUNTRY_IS_NU	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_NU)	// country == Niue
#define	RMM_COUNTRY_IS_NF	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_NF)	// country == Norfolk Island
#define	RMM_COUNTRY_IS_MP	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_MP)	// country == Northern Mariana Islands
#define	RMM_COUNTRY_IS_NO	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_NO)	// country == Norway

#define	RMM_COUNTRY_IS_OM	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_OM)	// country == Oman

#define	RMM_COUNTRY_IS_PK	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_PK)	// country == Pakistan
#define	RMM_COUNTRY_IS_PW	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_PW)	// country == Palau
#define	RMM_COUNTRY_IS_PS	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_PS)	// country == Palestinian Territory, Occupied
#define	RMM_COUNTRY_IS_PA	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_PA)	// country == Panama
#define	RMM_COUNTRY_IS_PG	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_PG)	// country == Papua New Guinea
#define	RMM_COUNTRY_IS_PY	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_PY)	// country == Paraguay
#define	RMM_COUNTRY_IS_PE	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_PE)	// country == Peru
#define	RMM_COUNTRY_IS_PH	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_PH)	// country == Philippines
#define	RMM_COUNTRY_IS_PN	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_PN)	// country == Pitcairn
#define	RMM_COUNTRY_IS_PL	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_PL)	// country == Poland
#define	RMM_COUNTRY_IS_PT	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_PT)	// country == Portugal
#define	RMM_COUNTRY_IS_PR	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_PR)	// country == Puerto Rico

#define	RMM_COUNTRY_IS_QA	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_QA)	// country == Qatar

#define	RMM_COUNTRY_IS_RE	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_RE)	// country == Reunion
#define	RMM_COUNTRY_IS_RO	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_RO)	// country == Romania
#define	RMM_COUNTRY_IS_RU	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_RU)	// country == Russian Federation
#define	RMM_COUNTRY_IS_RW	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_RW)	// country == Rwanda

#define	RMM_COUNTRY_IS_BL	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_BL)	// country == Saint Barthelemy
#define	RMM_COUNTRY_IS_SH	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_SH)	// country == Saint Helena
#define	RMM_COUNTRY_IS_KN	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_KN)	// country == Saint Kitts and Nevis
#define	RMM_COUNTRY_IS_LC	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_LC)	// country == Saint Lucia
#define	RMM_COUNTRY_IS_MF	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_MF)	// country == Saint Martin (French part)
#define	RMM_COUNTRY_IS_PM	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_PM)	// country == Saint Pierre and Miquelon
#define	RMM_COUNTRY_IS_VC	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_VC)	// country == Saint Vincent and the Grenadines
#define	RMM_COUNTRY_IS_WS	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_WS)	// country == Samoa
#define	RMM_COUNTRY_IS_SM	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_SM)	// country == San Marino
#define	RMM_COUNTRY_IS_ST	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_ST)	// country == Sao Tome and Principe
#define	RMM_COUNTRY_IS_SA	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_SA)	// country == Saudi Arabia
#define	RMM_COUNTRY_IS_SN	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_SN)	// country == Senegal
#define	RMM_COUNTRY_IS_RS	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_RS)	// country == Serbia
#define	RMM_COUNTRY_IS_SC	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_SC)	// country == Seychelles
#define	RMM_COUNTRY_IS_SL	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_SL)	// country == Sierra Leone
#define	RMM_COUNTRY_IS_SG	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_SG)	// country == Singapore
#define	RMM_COUNTRY_IS_SK	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_SK)	// country == Slovakia
#define	RMM_COUNTRY_IS_SI	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_SI)	// country == Slovenia
#define	RMM_COUNTRY_IS_SB	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_SB)	// country == Solomon Islands
#define	RMM_COUNTRY_IS_SO	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_SO)	// country == Somalia
#define	RMM_COUNTRY_IS_ZA	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_ZA)	// country == South Africa
#define	RMM_COUNTRY_IS_GS	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_GS)	// country == South Georgia and the South Sandwich Islands
#define	RMM_COUNTRY_IS_ES	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_ES)	// country == Spain
#define	RMM_COUNTRY_IS_LK	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_LK)	// country == Sri Lanka
#define	RMM_COUNTRY_IS_SD	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_SD)	// country == Sudan
#define	RMM_COUNTRY_IS_SR	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_SR)	// country == Suriname
#define	RMM_COUNTRY_IS_SJ	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_SJ)	// country == Svalbard and Jan Mayen
#define	RMM_COUNTRY_IS_SZ	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_SZ)	// country == Swaziland
#define	RMM_COUNTRY_IS_SE	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_SE)	// country == Sweden
#define	RMM_COUNTRY_IS_CH	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_CH)	// country == Switzerland
#define	RMM_COUNTRY_IS_SY	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_SY)	// country == Syrian Arab Republic

#define	RMM_COUNTRY_IS_TW	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_TW)	// country == Taiwan, Province of China
#define	RMM_COUNTRY_IS_TJ	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_TJ)	// country == Tajikistan
#define	RMM_COUNTRY_IS_TZ	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_TZ)	// country == Tanzania, United Republic of
#define	RMM_COUNTRY_IS_TH	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_TH)	// country == Thailand
#define	RMM_COUNTRY_IS_TL	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_TL)	// country == Timor-Leste
#define	RMM_COUNTRY_IS_TG	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_TG)	// country == Togo
#define	RMM_COUNTRY_IS_TK	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_TK)	// country == Tokelau
#define	RMM_COUNTRY_IS_TO	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_TO)	// country == Tonga
#define	RMM_COUNTRY_IS_TT	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_TT)	// country == Trinidad and Tobago
#define	RMM_COUNTRY_IS_TN	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_TN)	// country == Tunisia
#define	RMM_COUNTRY_IS_TR	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_TR)	// country == Turkey
#define	RMM_COUNTRY_IS_TM	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_TM)	// country == Turkmenistan
#define	RMM_COUNTRY_IS_TC	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_TC)	// country == Turks and Caicos Islands
#define	RMM_COUNTRY_IS_TV	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_TV)	// country == Tuvalu

#define	RMM_COUNTRY_IS_UG	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_UG)	// country == Uganda
#define	RMM_COUNTRY_IS_UA	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_UA)	// country == Ukraine
#define	RMM_COUNTRY_IS_AE	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_AE)	// country == United Arab Emirates
#define	RMM_COUNTRY_IS_UK	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_UK)	// country == United Kingdom (Exception: GB -> UK)
#define	RMM_COUNTRY_IS_US	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_US)	// country == United States
#define	RMM_COUNTRY_IS_UM	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_UM)	// country == United States Minor Outlying Islands
#define	RMM_COUNTRY_IS_UY	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_UY)	// country == Uruguay
#define	RMM_COUNTRY_IS_UZ	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_UZ)	// country == Uzbekistan

#define	RMM_COUNTRY_IS_VU	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_VU)	// country == Vanuatu
#define	RMM_COUNTRY_IS_VE	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_VE)	// country == Venezuela
#define	RMM_COUNTRY_IS_VN	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_VN)	// country == Viet Nam
#define	RMM_COUNTRY_IS_VG	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_VG)	// country == Virgin Islands, British
#define	RMM_COUNTRY_IS_VI	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_VI)	// country == Virgin Islands, U.S.

#define	RMM_COUNTRY_IS_WF	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_WF)	// country == Wallis and Futuna
#define	RMM_COUNTRY_IS_EH	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_EH)	// country == Western Sahara

#define	RMM_COUNTRY_IS_YE	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_YE)	// country == Yemen

#define	RMM_COUNTRY_IS_ZM	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_ZM)	// country == Zambia
#define	RMM_COUNTRY_IS_ZW	(RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_ZW)	// country == Zimbabwe

/* China City Option */
/*
    1. RMM_COUNTRY_IS_CN
    2. RMM_COUNTRY_IS_CNBJ,RMM_CNCITY_IS_BJ
        - If RMM_GROUP_CODE is CN and country(city) is Beijing, It's TRUE
    3. RMM_COUNTRY_IS_CNO,RMM_CNCITY_IS_CNO
        - If RMM_GROUP_CODE is CN and country(city) is Others, It's TRUE
*/
/*zhangze 20101120 added Hong Kong group*/
#define	RMM_COUNTRY_IS_CN					((RMM_GROUP_CODE==RMM_GROUP_CODE_CN || RMM_GROUP_CODE==RMM_GROUP_CODE_HK) && (RMM_COUNTRY_CODE!=RMM_COUNTRY_CODE_HK))	// country == China(CN group and not HK)
#define	RMM_COUNTRY_IS_CN_CHECK(country)	((RMM_GROUP_CODE==RMM_GROUP_CODE_CN || RMM_GROUP_CODE==RMM_GROUP_CODE_HK) && (country!=RMM_COUNTRY_CODE_HK))				// country == China(CN group and not HK)
#define RMM_COUNTRY_IS_CNBJ					((RMM_GROUP_CODE==RMM_GROUP_CODE_CN || RMM_GROUP_CODE==RMM_GROUP_CODE_HK) && (RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_CNBJ))	// group == China && Beijing
#define RMM_CNCITY_IS_BJ					RMM_COUNTRY_IS_CNBJ
//add by lihuifeng 2009.10.06 for city option.
#define RMM_COUNTRY_IS_CNCC  				((RMM_GROUP_CODE==RMM_GROUP_CODE_CN || RMM_GROUP_CODE==RMM_GROUP_CODE_HK) && (RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_CNCC))	
#define RMM_CNCITY_IS_CC					RMM_COUNTRY_IS_CNCC
#define RMM_COUNTRY_IS_CNCS					((RMM_GROUP_CODE==RMM_GROUP_CODE_CN || RMM_GROUP_CODE==RMM_GROUP_CODE_HK) && (RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_CNCS))
#define RMM_CNCITY_IS_CS					RMM_COUNTRY_IS_CNCS
#define RMM_COUNTRY_IS_CNCD					((RMM_GROUP_CODE==RMM_GROUP_CODE_CN || RMM_GROUP_CODE==RMM_GROUP_CODE_HK) && (RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_CNCD))
#define RMM_CNCITY_IS_CD					RMM_COUNTRY_IS_CNCD
#define RMM_COUNTRY_IS_CNDL					((RMM_GROUP_CODE==RMM_GROUP_CODE_CN || RMM_GROUP_CODE==RMM_GROUP_CODE_HK) && (RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_CNDL))
#define RMM_CNCITY_IS_DL					RMM_COUNTRY_IS_CNDL
#define RMM_COUNTRY_IS_CNFZ					((RMM_GROUP_CODE==RMM_GROUP_CODE_CN || RMM_GROUP_CODE==RMM_GROUP_CODE_HK) && (RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_CNFZ))
#define RMM_CNCITY_IS_FZ					RMM_COUNTRY_IS_CNFZ
#define RMM_COUNTRY_IS_CNGZ					((RMM_GROUP_CODE==RMM_GROUP_CODE_CN || RMM_GROUP_CODE==RMM_GROUP_CODE_HK) && (RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_CNGZ))
#define RMM_CNCITY_IS_GZ					RMM_COUNTRY_IS_CNGZ
#define RMM_COUNTRY_IS_CNHB					((RMM_GROUP_CODE==RMM_GROUP_CODE_CN || RMM_GROUP_CODE==RMM_GROUP_CODE_HK) && (RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_CNHB))
#define RMM_CNCITY_IS_HB					RMM_COUNTRY_IS_CNHB
#define RMM_COUNTRY_IS_CNHF					((RMM_GROUP_CODE==RMM_GROUP_CODE_CN || RMM_GROUP_CODE==RMM_GROUP_CODE_HK) && (RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_CNHF))
#define RMM_CNCITY_IS_HF					RMM_COUNTRY_IS_CNHF
#define RMM_COUNTRY_IS_CNJL					((RMM_GROUP_CODE==RMM_GROUP_CODE_CN || RMM_GROUP_CODE==RMM_GROUP_CODE_HK) && (RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_CNJL))  //yinxianghua   20091117
#define RMM_CNCITY_IS_JL					RMM_COUNTRY_IS_CNJL                                                                 //yinxianghua   20091117
#define RMM_COUNTRY_IS_CNNB					((RMM_GROUP_CODE==RMM_GROUP_CODE_CN || RMM_GROUP_CODE==RMM_GROUP_CODE_HK) && (RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_CNNB))
#define RMM_CNCITY_IS_NB					RMM_COUNTRY_IS_CNNB
#define RMM_COUNTRY_IS_CNSY					((RMM_GROUP_CODE==RMM_GROUP_CODE_CN || RMM_GROUP_CODE==RMM_GROUP_CODE_HK) && (RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_CNSY))
#define RMM_CNCITY_IS_SY					RMM_COUNTRY_IS_CNSY
#define RMM_COUNTRY_IS_CNWH					((RMM_GROUP_CODE==RMM_GROUP_CODE_CN || RMM_GROUP_CODE==RMM_GROUP_CODE_HK) && (RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_CNWH))
#define RMM_CNCITY_IS_WH					RMM_COUNTRY_IS_CNWH
#define RMM_COUNTRY_IS_CNXA					((RMM_GROUP_CODE==RMM_GROUP_CODE_CN || RMM_GROUP_CODE==RMM_GROUP_CODE_HK) && (RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_CNXA))	
#define RMM_CNCITY_IS_XA					RMM_COUNTRY_IS_CNXA
#define RMM_COUNTRY_IS_CNXM					((RMM_GROUP_CODE==RMM_GROUP_CODE_CN || RMM_GROUP_CODE==RMM_GROUP_CODE_HK) && (RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_CNXM))
#define RMM_CNCITY_IS_XM					RMM_COUNTRY_IS_CNXM
#define RMM_COUNTRY_IS_CNZZ					((RMM_GROUP_CODE==RMM_GROUP_CODE_CN || RMM_GROUP_CODE==RMM_GROUP_CODE_HK) && (RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_CNZZ))  //yinxianghua   20091203
#define RMM_CNCITY_IS_ZZ					RMM_COUNTRY_IS_CNZZ																    //yinxianghua   20091203
#define RMM_COUNTRY_IS_CNZH					((RMM_GROUP_CODE==RMM_GROUP_CODE_CN || RMM_GROUP_CODE==RMM_GROUP_CODE_HK) && (RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_CNZH))
#define RMM_CNCITY_IS_ZH					RMM_COUNTRY_IS_CNZH
//end
#define RMM_COUNTRY_IS_CNO					((RMM_GROUP_CODE==RMM_GROUP_CODE_CN || RMM_GROUP_CODE==RMM_GROUP_CODE_HK) && (RMM_COUNTRY_CODE==RMM_COUNTRY_CODE_CNO))	// group == China && China Mainland Others
#define RMM_CNCITY_IS_CNO					RMM_COUNTRY_IS_CNO

#ifdef __cplusplus
}
#endif
#endif	/* _COUNTRY_CONDITIONS_H_ */
