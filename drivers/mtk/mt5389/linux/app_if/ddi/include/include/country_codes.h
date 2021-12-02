/******************************************************************************
 *	 DTV LABORATORY, LG ELECTRONICS INC., SEOUL, KOREA
 *	 Copyright(c) 2008 by LG Electronics Inc.
 *
 *	 All rights reserved. No part of this work may be reproduced, stored in a
 *	 retrieval system, or transmitted by any means without prior written
 *	 permission of LG Electronics Inc.
 *****************************************************************************/

/** @file country_codes.h
 *
 * This header file defines the macros definitions defining country code
 *
 *	@author 	David (DONG-IL) Lee
 *	@version	1.0
 *	@date		2008.05.08
 *	@note		This file shall be included only in country.h
 *	@see		ISO 3166-1 (Exception: UK <- GB)
 */

#ifndef WIN32_SIM
#ifndef WIN32_SIM
#ifndef _COUNTRY_H_
	#error	NOTE: This file should be included only in country.h
#endif
#endif
#endif


#ifndef	_COUNTRY_CODES_H_
#define	_COUNTRY_CODES_H_
#ifdef __cplusplus
extern "C" {
#endif

#define	RMM_COUNTRY_CODE_AF	RMM_CODE_VAL_CC(A,F,G)	// Afghanistan
#define	RMM_COUNTRY_CODE_AX	RMM_CODE_VAL_CC(A,L,A)	// Aland Islands
#define	RMM_COUNTRY_CODE_AL	RMM_CODE_VAL_CC(A,L,B)	// Albania
#define	RMM_COUNTRY_CODE_DZ	RMM_CODE_VAL_CC(D,Z,A)	// Algeria
#define	RMM_COUNTRY_CODE_AS	RMM_CODE_VAL_CC(A,S,M)	// American Samoa
#define	RMM_COUNTRY_CODE_AD	RMM_CODE_VAL_CC(A,N,D)	// Andorra
#define	RMM_COUNTRY_CODE_AO	RMM_CODE_VAL_CC(A,G,O)	// Angola
#define	RMM_COUNTRY_CODE_AI	RMM_CODE_VAL_CC(A,I,A)	// Anguilla
#define	RMM_COUNTRY_CODE_AQ	RMM_CODE_VAL_CC(A,T,A)	// Antarctica
#define	RMM_COUNTRY_CODE_AG	RMM_CODE_VAL_CC(A,T,G)	// Antigua and Barbuda
#define	RMM_COUNTRY_CODE_AR	RMM_CODE_VAL_CC(A,R,G)	// Argentina
#define	RMM_COUNTRY_CODE_AM	RMM_CODE_VAL_CC(A,R,M)	// Armenia
#define	RMM_COUNTRY_CODE_AW	RMM_CODE_VAL_CC(A,B,W)	// Aruba
#define	RMM_COUNTRY_CODE_AU	RMM_CODE_VAL_CC(A,U,S)	// Australia
#define	RMM_COUNTRY_CODE_AT	RMM_CODE_VAL_CC(A,U,T)	// Austria
#define	RMM_COUNTRY_CODE_AZ	RMM_CODE_VAL_CC(A,Z,E)	// Azerbaijan

#define	RMM_COUNTRY_CODE_BS	RMM_CODE_VAL_CC(B,H,S)	// Bahamas
#define	RMM_COUNTRY_CODE_BH	RMM_CODE_VAL_CC(B,H,R)	// Bahrain
#define	RMM_COUNTRY_CODE_BD	RMM_CODE_VAL_CC(B,G,D)	// Bangladesh
#define	RMM_COUNTRY_CODE_BB	RMM_CODE_VAL_CC(B,R,B)	// Barbados
#define	RMM_COUNTRY_CODE_BY	RMM_CODE_VAL_CC(B,L,R)	// Belarus
#define	RMM_COUNTRY_CODE_BE	RMM_CODE_VAL_CC(B,E,L)	// Belgium
#define	RMM_COUNTRY_CODE_BZ	RMM_CODE_VAL_CC(B,L,Z)	// Belize
#define	RMM_COUNTRY_CODE_BJ	RMM_CODE_VAL_CC(B,E,N)	// Benin
#define	RMM_COUNTRY_CODE_BM	RMM_CODE_VAL_CC(B,M,U)	// Bermuda
#define	RMM_COUNTRY_CODE_BT	RMM_CODE_VAL_CC(B,T,N)	// Bhutan
#define	RMM_COUNTRY_CODE_BO	RMM_CODE_VAL_CC(B,O,L)	// Bolivia
#define	RMM_COUNTRY_CODE_BA	RMM_CODE_VAL_CC(B,I,H)	// Bosnia and Herzegovina
#define	RMM_COUNTRY_CODE_BW	RMM_CODE_VAL_CC(B,W,A)	// Botswana
#define	RMM_COUNTRY_CODE_BV	RMM_CODE_VAL_CC(B,V,T)	// Bouvet Island
#define	RMM_COUNTRY_CODE_BR	RMM_CODE_VAL_CC(B,R,A)	// Brazil
#define	RMM_COUNTRY_CODE_IO	RMM_CODE_VAL_CC(I,O,T)	// British Indian Ocean Territory
#define	RMM_COUNTRY_CODE_BN	RMM_CODE_VAL_CC(B,R,N)	// Brunei Darussalam
#define	RMM_COUNTRY_CODE_BG	RMM_CODE_VAL_CC(B,G,R)	// Bulgaria
#define	RMM_COUNTRY_CODE_BF	RMM_CODE_VAL_CC(B,F,A)	// Burkina Faso
#define	RMM_COUNTRY_CODE_BI	RMM_CODE_VAL_CC(B,D,I)	// Burundi

#define	RMM_COUNTRY_CODE_KH	RMM_CODE_VAL_CC(K,H,M)	// Cambodia
#define	RMM_COUNTRY_CODE_CM	RMM_CODE_VAL_CC(C,M,R)	// Cameroon
#define	RMM_COUNTRY_CODE_CA	RMM_CODE_VAL_CC(C,A,N)	// Canada
#define	RMM_COUNTRY_CODE_CV	RMM_CODE_VAL_CC(C,P,V)	// Cape Verde
#define	RMM_COUNTRY_CODE_KY	RMM_CODE_VAL_CC(C,Y,M)	// Cayman Islands
#define	RMM_COUNTRY_CODE_CF	RMM_CODE_VAL_CC(C,A,F)	// Central African Republic
#define	RMM_COUNTRY_CODE_TD	RMM_CODE_VAL_CC(T,C,D)	// Chad
#define	RMM_COUNTRY_CODE_CL	RMM_CODE_VAL_CC(C,H,L)	// Chile
//#define	RMM_COUNTRY_CODE_CN	RMM_CODE_VAL_CC(C,H,N)	// China
#define	RMM_COUNTRY_CODE_CX	RMM_CODE_VAL_CC(C,X,R)	// Christmas Island
#define	RMM_COUNTRY_CODE_CC	RMM_CODE_VAL_CC(C,C,K)	// Cocos (Keeling) Islands
#define	RMM_COUNTRY_CODE_CO	RMM_CODE_VAL_CC(C,O,L)	// Colombia
#define	RMM_COUNTRY_CODE_KM	RMM_CODE_VAL_CC(C,O,M)	// Comoros
#define	RMM_COUNTRY_CODE_CG	RMM_CODE_VAL_CC(C,O,G)	// Congo
#define	RMM_COUNTRY_CODE_CD	RMM_CODE_VAL_CC(C,O,D)	// Congo, Democratic Republic of the
#define	RMM_COUNTRY_CODE_CK	RMM_CODE_VAL_CC(C,O,K)	// Cook Islands
#define	RMM_COUNTRY_CODE_CR	RMM_CODE_VAL_CC(C,R,I)	// Costa Rica
#define	RMM_COUNTRY_CODE_CI	RMM_CODE_VAL_CC(C,I,V)	// Cote d'Ivoire
#define	RMM_COUNTRY_CODE_HR	RMM_CODE_VAL_CC(H,R,V)	// Croatia
#define	RMM_COUNTRY_CODE_CU	RMM_CODE_VAL_CC(C,U,B)	// Cuba
#define	RMM_COUNTRY_CODE_CY	RMM_CODE_VAL_CC(C,Y,P)	// Cyprus
#define	RMM_COUNTRY_CODE_CZ	RMM_CODE_VAL_CC(C,Z,E)	// Czech Republic

#define	RMM_COUNTRY_CODE_DK	RMM_CODE_VAL_CC(D,N,K)	// Denmark
#define	RMM_COUNTRY_CODE_DJ	RMM_CODE_VAL_CC(D,J,I)	// Djibouti
#define	RMM_COUNTRY_CODE_DM	RMM_CODE_VAL_CC(D,M,A)	// Dominica
#define	RMM_COUNTRY_CODE_DO	RMM_CODE_VAL_CC(D,O,M)	// Dominican Republic

#define	RMM_COUNTRY_CODE_EC	RMM_CODE_VAL_CC(E,C,U)	// Ecuador
#define	RMM_COUNTRY_CODE_EG	RMM_CODE_VAL_CC(E,G,Y)	// Egypt
#define	RMM_COUNTRY_CODE_SV	RMM_CODE_VAL_CC(S,L,V)	// El Salvador
#define	RMM_COUNTRY_CODE_GQ	RMM_CODE_VAL_CC(G,N,Q)	// Equatorial Guinea
#define	RMM_COUNTRY_CODE_ER	RMM_CODE_VAL_CC(E,R,I)	// Eritrea
#define	RMM_COUNTRY_CODE_EE	RMM_CODE_VAL_CC(E,S,T)	// Estonia
#define	RMM_COUNTRY_CODE_ET	RMM_CODE_VAL_CC(E,T,H)	// Ethiopia

#define	RMM_COUNTRY_CODE_FK	RMM_CODE_VAL_CC(F,L,K)	// Falkland Islands (Malvinas)
#define	RMM_COUNTRY_CODE_FO	RMM_CODE_VAL_CC(F,R,O)	// Faroe Islands
#define	RMM_COUNTRY_CODE_FJ	RMM_CODE_VAL_CC(F,J,I)	// Fiji
#define	RMM_COUNTRY_CODE_FI	RMM_CODE_VAL_CC(F,I,N)	// Finland
#define	RMM_COUNTRY_CODE_FR	RMM_CODE_VAL_CC(F,R,A)	// France
#define	RMM_COUNTRY_CODE_GF	RMM_CODE_VAL_CC(G,U,F)	// French Guiana
#define	RMM_COUNTRY_CODE_PF	RMM_CODE_VAL_CC(P,Y,F)	// French Polynesia
#define	RMM_COUNTRY_CODE_TF	RMM_CODE_VAL_CC(A,T,F)	// French Southern Territories

#define	RMM_COUNTRY_CODE_GA	RMM_CODE_VAL_CC(G,A,B)	// Gabon
#define	RMM_COUNTRY_CODE_GM	RMM_CODE_VAL_CC(G,M,B)	// Gambia
#define	RMM_COUNTRY_CODE_GE	RMM_CODE_VAL_CC(G,E,O)	// Georgia
#define	RMM_COUNTRY_CODE_DE	RMM_CODE_VAL_CC(D,E,U)	// Germany
#define	RMM_COUNTRY_CODE_GH	RMM_CODE_VAL_CC(G,H,A)	// Ghana
#define	RMM_COUNTRY_CODE_GI	RMM_CODE_VAL_CC(G,I,B)	// Gibraltar
#define	RMM_COUNTRY_CODE_GR	RMM_CODE_VAL_CC(G,R,C)	// Greece
#define	RMM_COUNTRY_CODE_GL	RMM_CODE_VAL_CC(G,R,L)	// Greenland
#define	RMM_COUNTRY_CODE_GD	RMM_CODE_VAL_CC(G,R,D)	// Grenada
#define	RMM_COUNTRY_CODE_GP	RMM_CODE_VAL_CC(G,L,P)	// Guadeloupe
#define	RMM_COUNTRY_CODE_GU	RMM_CODE_VAL_CC(G,U,M)	// Guam
#define	RMM_COUNTRY_CODE_GT	RMM_CODE_VAL_CC(G,T,M)	// Guatemala
#define	RMM_COUNTRY_CODE_GG	RMM_CODE_VAL_CC(G,G,Y)	// Guernsey
#define	RMM_COUNTRY_CODE_GN	RMM_CODE_VAL_CC(G,I,N)	// Guinea
#define	RMM_COUNTRY_CODE_GW	RMM_CODE_VAL_CC(G,N,B)	// Guinea-Bissau
#define	RMM_COUNTRY_CODE_GY	RMM_CODE_VAL_CC(G,U,Y)	// Guyana

#define	RMM_COUNTRY_CODE_HT	RMM_CODE_VAL_CC(H,T,I)	// Haiti
#define	RMM_COUNTRY_CODE_HM	RMM_CODE_VAL_CC(H,M,D)	// Heard Island and McDonald Islands
#define	RMM_COUNTRY_CODE_VA	RMM_CODE_VAL_CC(V,A,T)	// Holy See (Vatican City State)
#define	RMM_COUNTRY_CODE_HN	RMM_CODE_VAL_CC(H,N,D)	// Honduras
#define	RMM_COUNTRY_CODE_HK	RMM_CODE_VAL_CC(H,K,G)	// Hong Kong
#define	RMM_COUNTRY_CODE_HU	RMM_CODE_VAL_CC(H,U,N)	// Hungary

#define	RMM_COUNTRY_CODE_IS	RMM_CODE_VAL_CC(I,S,L)	// Iceland
#define	RMM_COUNTRY_CODE_IN	RMM_CODE_VAL_CC(I,N,D)	// India
#define	RMM_COUNTRY_CODE_ID	RMM_CODE_VAL_CC(I,D,N)	// Indonesia
#define	RMM_COUNTRY_CODE_IR	RMM_CODE_VAL_CC(I,R,N)	// Iran, Islamic Republic of
#define	RMM_COUNTRY_CODE_IQ	RMM_CODE_VAL_CC(I,R,Q)	// Iraq
#define	RMM_COUNTRY_CODE_IE	RMM_CODE_VAL_CC(I,R,L)	// Ireland
#define	RMM_COUNTRY_CODE_IM	RMM_CODE_VAL_CC(I,M,N)	// Isle of Man
#define	RMM_COUNTRY_CODE_IL	RMM_CODE_VAL_CC(I,S,R)	// Israel
#define	RMM_COUNTRY_CODE_IT	RMM_CODE_VAL_CC(I,T,A)	// Italy

#define	RMM_COUNTRY_CODE_JM	RMM_CODE_VAL_CC(J,A,M)	// Jamaica
#define	RMM_COUNTRY_CODE_JP	RMM_CODE_VAL_CC(J,P,N)	// Japan
#define	RMM_COUNTRY_CODE_JE	RMM_CODE_VAL_CC(J,E,Y)	// Jersey
#define	RMM_COUNTRY_CODE_JO	RMM_CODE_VAL_CC(J,O,R)	// Jordan

#define	RMM_COUNTRY_CODE_KZ	RMM_CODE_VAL_CC(K,A,Z)	// Kazakhstan
#define	RMM_COUNTRY_CODE_KE	RMM_CODE_VAL_CC(K,E,N)	// Kenya
#define	RMM_COUNTRY_CODE_KI	RMM_CODE_VAL_CC(K,I,R)	// Kiribati
#define	RMM_COUNTRY_CODE_KP	RMM_CODE_VAL_CC(P,R,K)	// Korea, Democratic People's Republic of
#define	RMM_COUNTRY_CODE_KR	RMM_CODE_VAL_CC(K,O,R)	// Korea, Republic of
#define	RMM_COUNTRY_CODE_KW	RMM_CODE_VAL_CC(K,W,T)	// Kuwait
#define	RMM_COUNTRY_CODE_KG	RMM_CODE_VAL_CC(K,G,Z)	// Kyrgyzstan

#define	RMM_COUNTRY_CODE_LA	RMM_CODE_VAL_CC(L,A,O)	// Lao People's Democratic Republic
#define	RMM_COUNTRY_CODE_LV	RMM_CODE_VAL_CC(L,V,A)	// Latvia
#define	RMM_COUNTRY_CODE_LB	RMM_CODE_VAL_CC(L,B,N)	// Lebanon
#define	RMM_COUNTRY_CODE_LS	RMM_CODE_VAL_CC(L,S,O)	// Lesotho
#define	RMM_COUNTRY_CODE_LR	RMM_CODE_VAL_CC(L,B,R)	// Liberia
#define	RMM_COUNTRY_CODE_LY	RMM_CODE_VAL_CC(L,B,Y)	// Libyan Arab Jamahiriya
#define	RMM_COUNTRY_CODE_LI	RMM_CODE_VAL_CC(L,I,E)	// Liechtenstein
#define	RMM_COUNTRY_CODE_LT	RMM_CODE_VAL_CC(L,T,U)	// Lithuania
#define	RMM_COUNTRY_CODE_LU	RMM_CODE_VAL_CC(L,U,X)	// Luxembourg

#define	RMM_COUNTRY_CODE_MO	RMM_CODE_VAL_CC(M,A,C)	// Macao
#define	RMM_COUNTRY_CODE_MK	RMM_CODE_VAL_CC(M,K,D)	// Macedonia, the former Yugoslav Republic of
#define	RMM_COUNTRY_CODE_MG	RMM_CODE_VAL_CC(M,D,G)	// Madagascar
#define	RMM_COUNTRY_CODE_MW	RMM_CODE_VAL_CC(M,W,I)	// Malawi
#define	RMM_COUNTRY_CODE_MY	RMM_CODE_VAL_CC(M,Y,S)	// Malaysia
#define	RMM_COUNTRY_CODE_MV	RMM_CODE_VAL_CC(M,D,V)	// Maldives
#define	RMM_COUNTRY_CODE_ML	RMM_CODE_VAL_CC(M,L,I)	// Mali
#define	RMM_COUNTRY_CODE_MT	RMM_CODE_VAL_CC(M,L,T)	// Malta
#define	RMM_COUNTRY_CODE_MH	RMM_CODE_VAL_CC(M,H,L)	// Marshall Islands
#define	RMM_COUNTRY_CODE_MQ	RMM_CODE_VAL_CC(M,T,Q)	// Martinique
#define	RMM_COUNTRY_CODE_MR	RMM_CODE_VAL_CC(M,R,T)	// Mauritania
#define	RMM_COUNTRY_CODE_MU	RMM_CODE_VAL_CC(M,U,S)	// Mauritius
#define	RMM_COUNTRY_CODE_YT	RMM_CODE_VAL_CC(M,Y,T)	// Mayotte
#define	RMM_COUNTRY_CODE_MX	RMM_CODE_VAL_CC(M,E,X)	// Mexico
#define	RMM_COUNTRY_CODE_FM	RMM_CODE_VAL_CC(F,S,M)	// Micronesia, Federated States of
#define	RMM_COUNTRY_CODE_MD	RMM_CODE_VAL_CC(M,D,A)	// Moldova
#define	RMM_COUNTRY_CODE_MC	RMM_CODE_VAL_CC(M,C,O)	// Monaco
#define	RMM_COUNTRY_CODE_MN	RMM_CODE_VAL_CC(M,N,G)	// Mongolia
#define	RMM_COUNTRY_CODE_ME	RMM_CODE_VAL_CC(M,N,E)	// Montenegro
#define	RMM_COUNTRY_CODE_MS	RMM_CODE_VAL_CC(M,S,R)	// Montserrat
#define	RMM_COUNTRY_CODE_MA	RMM_CODE_VAL_CC(M,A,R)	// Morocco
#define	RMM_COUNTRY_CODE_MZ	RMM_CODE_VAL_CC(M,O,Z)	// Mozambique
#define	RMM_COUNTRY_CODE_MM	RMM_CODE_VAL_CC(M,M,R)	// Myanmar

#define	RMM_COUNTRY_CODE_NA	RMM_CODE_VAL_CC(N,A,M)	// Namibia
#define	RMM_COUNTRY_CODE_NR	RMM_CODE_VAL_CC(N,R,U)	// Nauru
#define	RMM_COUNTRY_CODE_NP	RMM_CODE_VAL_CC(N,P,L)	// Nepal
#define	RMM_COUNTRY_CODE_NL	RMM_CODE_VAL_CC(N,L,D)	// Netherlands
#define	RMM_COUNTRY_CODE_AN	RMM_CODE_VAL_CC(A,N,T)	// Netherlands Antilles
#define	RMM_COUNTRY_CODE_NC	RMM_CODE_VAL_CC(N,C,L)	// New Caledonia
#define	RMM_COUNTRY_CODE_NZ	RMM_CODE_VAL_CC(N,Z,L)	// New Zealand
#define	RMM_COUNTRY_CODE_NI	RMM_CODE_VAL_CC(N,I,C)	// Nicaragua
#define	RMM_COUNTRY_CODE_NE	RMM_CODE_VAL_CC(N,E,R)	// Niger
#define	RMM_COUNTRY_CODE_NG	RMM_CODE_VAL_CC(N,G,A)	// Nigeria
#define	RMM_COUNTRY_CODE_NU	RMM_CODE_VAL_CC(N,I,U)	// Niue
#define	RMM_COUNTRY_CODE_NF	RMM_CODE_VAL_CC(N,F,K)	// Norfolk Island
#define	RMM_COUNTRY_CODE_MP	RMM_CODE_VAL_CC(M,N,P)	// Northern Mariana Islands
#define	RMM_COUNTRY_CODE_NO	RMM_CODE_VAL_CC(N,O,R)	// Norway

#define	RMM_COUNTRY_CODE_OM	RMM_CODE_VAL_CC(O,M,N)	// Oman

#define	RMM_COUNTRY_CODE_PK	RMM_CODE_VAL_CC(P,A,K)	// Pakistan
#define	RMM_COUNTRY_CODE_PW	RMM_CODE_VAL_CC(P,L,W)	// Palau
#define	RMM_COUNTRY_CODE_PS	RMM_CODE_VAL_CC(P,S,E)	// Palestinian Territory, Occupied
#define	RMM_COUNTRY_CODE_PA	RMM_CODE_VAL_CC(P,A,N)	// Panama
#define	RMM_COUNTRY_CODE_PG	RMM_CODE_VAL_CC(P,N,G)	// Papua New Guinea
#define	RMM_COUNTRY_CODE_PY	RMM_CODE_VAL_CC(P,R,Y)	// Paraguay
#define	RMM_COUNTRY_CODE_PE	RMM_CODE_VAL_CC(P,E,R)	// Peru
#define	RMM_COUNTRY_CODE_PH	RMM_CODE_VAL_CC(P,H,L)	// Philippines
#define	RMM_COUNTRY_CODE_PN	RMM_CODE_VAL_CC(P,C,N)	// Pitcairn
#define	RMM_COUNTRY_CODE_PL	RMM_CODE_VAL_CC(P,O,L)	// Poland
#define	RMM_COUNTRY_CODE_PT	RMM_CODE_VAL_CC(P,R,T)	// Portugal
#define	RMM_COUNTRY_CODE_PR	RMM_CODE_VAL_CC(P,R,I)	// Puerto Rico

#define	RMM_COUNTRY_CODE_QA	RMM_CODE_VAL_CC(Q,A,T)	// Qatar

#define	RMM_COUNTRY_CODE_RE	RMM_CODE_VAL_CC(R,E,U)	// Reunion
#define	RMM_COUNTRY_CODE_RO	RMM_CODE_VAL_CC(R,O,U)	// Romania
#define	RMM_COUNTRY_CODE_RU	RMM_CODE_VAL_CC(R,U,S)	// Russian Federation
#define	RMM_COUNTRY_CODE_RW	RMM_CODE_VAL_CC(R,W,A)	// Rwanda

#define	RMM_COUNTRY_CODE_BL	RMM_CODE_VAL_CC(B,L,M)	// Saint Barthelemy
#define	RMM_COUNTRY_CODE_SH	RMM_CODE_VAL_CC(S,H,N)	// Saint Helena
#define	RMM_COUNTRY_CODE_KN	RMM_CODE_VAL_CC(K,N,A)	// Saint Kitts and Nevis
#define	RMM_COUNTRY_CODE_LC	RMM_CODE_VAL_CC(L,C,A)	// Saint Lucia
#define	RMM_COUNTRY_CODE_MF	RMM_CODE_VAL_CC(M,A,F)	// Saint Martin (French part)
#define	RMM_COUNTRY_CODE_PM	RMM_CODE_VAL_CC(S,P,M)	// Saint Pierre and Miquelon
#define	RMM_COUNTRY_CODE_VC	RMM_CODE_VAL_CC(V,C,T)	// Saint Vincent and the Grenadines
#define	RMM_COUNTRY_CODE_WS	RMM_CODE_VAL_CC(W,S,M)	// Samoa
#define	RMM_COUNTRY_CODE_SM	RMM_CODE_VAL_CC(S,M,R)	// San Marino
#define	RMM_COUNTRY_CODE_ST	RMM_CODE_VAL_CC(S,T,P)	// Sao Tome and Principe
#define	RMM_COUNTRY_CODE_SA	RMM_CODE_VAL_CC(S,A,U)	// Saudi Arabia
#define	RMM_COUNTRY_CODE_SN	RMM_CODE_VAL_CC(S,E,N)	// Senegal
#define	RMM_COUNTRY_CODE_RS	RMM_CODE_VAL_CC(S,R,B)	// Serbia
#define	RMM_COUNTRY_CODE_SC	RMM_CODE_VAL_CC(S,Y,C)	// Seychelles
#define	RMM_COUNTRY_CODE_SL	RMM_CODE_VAL_CC(S,L,E)	// Sierra Leone
#define	RMM_COUNTRY_CODE_SG	RMM_CODE_VAL_CC(S,G,P)	// Singapore
#define	RMM_COUNTRY_CODE_SK	RMM_CODE_VAL_CC(S,V,K)	// Slovakia
#define	RMM_COUNTRY_CODE_SI	RMM_CODE_VAL_CC(S,V,N)	// Slovenia
#define	RMM_COUNTRY_CODE_SB	RMM_CODE_VAL_CC(S,L,B)	// Solomon Islands
#define	RMM_COUNTRY_CODE_SO	RMM_CODE_VAL_CC(S,O,M)	// Somalia
#define	RMM_COUNTRY_CODE_ZA	RMM_CODE_VAL_CC(Z,A,F)	// South Africa
#define	RMM_COUNTRY_CODE_GS	RMM_CODE_VAL_CC(S,G,S)	// South Georgia and the South Sandwich Islands
#define	RMM_COUNTRY_CODE_ES	RMM_CODE_VAL_CC(E,S,P)	// Spain
#define	RMM_COUNTRY_CODE_LK	RMM_CODE_VAL_CC(L,K,A)	// Sri Lanka
#define	RMM_COUNTRY_CODE_SD	RMM_CODE_VAL_CC(S,D,N)	// Sudan
#define	RMM_COUNTRY_CODE_SR	RMM_CODE_VAL_CC(S,U,R)	// Suriname
#define	RMM_COUNTRY_CODE_SJ	RMM_CODE_VAL_CC(S,J,M)	// Svalbard and Jan Mayen
#define	RMM_COUNTRY_CODE_SZ	RMM_CODE_VAL_CC(S,W,Z)	// Swaziland
#define	RMM_COUNTRY_CODE_SE	RMM_CODE_VAL_CC(S,W,E)	// Sweden
#define	RMM_COUNTRY_CODE_CH	RMM_CODE_VAL_CC(C,H,E)	// Switzerland
#define	RMM_COUNTRY_CODE_SY	RMM_CODE_VAL_CC(S,Y,R)	// Syrian Arab Republic

#define	RMM_COUNTRY_CODE_TW	RMM_CODE_VAL_CC(T,W,N)	// Taiwan, Province of China
#define	RMM_COUNTRY_CODE_TJ	RMM_CODE_VAL_CC(T,J,K)	// Tajikistan
#define	RMM_COUNTRY_CODE_TZ	RMM_CODE_VAL_CC(T,Z,A)	// Tanzania, United Republic of
#define	RMM_COUNTRY_CODE_TH	RMM_CODE_VAL_CC(T,H,A)	// Thailand
#define	RMM_COUNTRY_CODE_TL	RMM_CODE_VAL_CC(T,L,S)	// Timor-Leste
#define	RMM_COUNTRY_CODE_TG	RMM_CODE_VAL_CC(T,G,O)	// Togo
#define	RMM_COUNTRY_CODE_TK	RMM_CODE_VAL_CC(T,K,L)	// Tokelau
#define	RMM_COUNTRY_CODE_TO	RMM_CODE_VAL_CC(T,O,N)	// Tonga
#define	RMM_COUNTRY_CODE_TT	RMM_CODE_VAL_CC(T,T,O)	// Trinidad and Tobago
#define	RMM_COUNTRY_CODE_TN	RMM_CODE_VAL_CC(T,U,N)	// Tunisia
#define	RMM_COUNTRY_CODE_TR	RMM_CODE_VAL_CC(T,U,R)	// Turkey
#define	RMM_COUNTRY_CODE_TM	RMM_CODE_VAL_CC(T,K,M)	// Turkmenistan
#define	RMM_COUNTRY_CODE_TC	RMM_CODE_VAL_CC(T,C,A)	// Turks and Caicos Islands
#define	RMM_COUNTRY_CODE_TV	RMM_CODE_VAL_CC(T,U,V)	// Tuvalu

#define	RMM_COUNTRY_CODE_UG	RMM_CODE_VAL_CC(U,G,A)	// Uganda
#define	RMM_COUNTRY_CODE_UA	RMM_CODE_VAL_CC(U,K,R)	// Ukraine
#define	RMM_COUNTRY_CODE_AE	RMM_CODE_VAL_CC(A,R,E)	// United Arab Emirates
#define	RMM_COUNTRY_CODE_UK	RMM_CODE_VAL_CC(G,B,R)	// United Kingdom (Originally GB, but replaced with UK)
#define	RMM_COUNTRY_CODE_US	RMM_CODE_VAL_CC(U,S,A)	// United States
#define	RMM_COUNTRY_CODE_UM	RMM_CODE_VAL_CC(U,M,I)	// United States Minor Outlying Islands
#define	RMM_COUNTRY_CODE_UY	RMM_CODE_VAL_CC(U,R,Y)	// Uruguay
#define	RMM_COUNTRY_CODE_UZ	RMM_CODE_VAL_CC(U,Z,B)	// Uzbekistan

#define	RMM_COUNTRY_CODE_VU	RMM_CODE_VAL_CC(V,U,T)	// Vanuatu
#define	RMM_COUNTRY_CODE_VE	RMM_CODE_VAL_CC(V,E,N)	// Venezuela
#define	RMM_COUNTRY_CODE_VN	RMM_CODE_VAL_CC(V,N,M)	// Viet Nam
#define	RMM_COUNTRY_CODE_VG	RMM_CODE_VAL_CC(V,G,B)	// Virgin Islands, British
#define	RMM_COUNTRY_CODE_VI	RMM_CODE_VAL_CC(V,I,R)	// Virgin Islands, U.S.

#define	RMM_COUNTRY_CODE_WF	RMM_CODE_VAL_CC(W,L,F)	// Wallis and Futuna
#define	RMM_COUNTRY_CODE_EH	RMM_CODE_VAL_CC(E,S,H)	// Western Sahara

#define	RMM_COUNTRY_CODE_YE	RMM_CODE_VAL_CC(Y,E,M)	// Yemen

#define	RMM_COUNTRY_CODE_ZM	RMM_CODE_VAL_CC(Z,M,B)	// Zambia
#define	RMM_COUNTRY_CODE_ZW	RMM_CODE_VAL_CC(Z,W,E)	// Zimbabwe

#ifdef __cplusplus
}
#endif
#endif	/* _COUNTRY_CODES_H_ */
