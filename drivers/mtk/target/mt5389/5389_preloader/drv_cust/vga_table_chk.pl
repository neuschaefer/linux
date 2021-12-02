#!/usr/local/bin/perl
###############################################################################
# Copyright Statement:                                                        #
#                                                                             #
#   This software/firmware and related documentation ("MediaTek Software")    #
# are protected under international and related jurisdictions'copyright laws  #
# as unpublished works. The information contained herein is confidential and  #
# proprietary to MediaTek Inc. Without the prior written permission of        #
# MediaTek Inc., any reproduction, modification, use or disclosure of         #
# MediaTek Software, and information contained herein, in whole or in part,   #
# shall be strictly prohibited.                                               #
# MediaTek Inc. Copyright (C) 2010. All rights reserved.                      #
#                                                                             #
#   BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND      #
# AGREES TO THE FOLLOWING:                                                    #
#                                                                             #
#   1)Any and all intellectual property rights (including without             #
# limitation, patent, copyright, and trade secrets) in and to this            #
# Software/firmware and related documentation ("MediaTek Software") shall     #
# remain the exclusive property of MediaTek Inc. Any and all intellectual     #
# property rights (including without limitation, patent, copyright, and       #
# trade secrets) in and to any modifications and derivatives to MediaTek      #
# Software, whoever made, shall also remain the exclusive property of         #
# MediaTek Inc.  Nothing herein shall be construed as any transfer of any     #
# title to any intellectual property right in MediaTek Software to Receiver.  #
#                                                                             #
#   2)This MediaTek Software Receiver received from MediaTek Inc. and/or its  #
# representatives is provided to Receiver on an "AS IS" basis only.           #
# MediaTek Inc. expressly disclaims all warranties, expressed or implied,     #
# including but not limited to any implied warranties of merchantability,     #
# non-infringement and fitness for a particular purpose and any warranties    #
# arising out of course of performance, course of dealing or usage of trade.  #
# MediaTek Inc. does not provide any warranty whatsoever with respect to the  #
# software of any third party which may be used by, incorporated in, or       #
# supplied with the MediaTek Software, and Receiver agrees to look only to    #
# such third parties for any warranty claim relating thereto.  Receiver       #
# expressly acknowledges that it is Receiver's sole responsibility to obtain  #
# from any third party all proper licenses contained in or delivered with     #
# MediaTek Software.  MediaTek is not responsible for any MediaTek Software   #
# releases made to Receiver's specifications or to conform to a particular    #
# standard or open forum.                                                     #
#                                                                             #
#   3)Receiver further acknowledge that Receiver may, either presently        #
# and/or in the future, instruct MediaTek Inc. to assist it in the            #
# development and the implementation, in accordance with Receiver's designs,  #
# of certain softwares relating to Receiver's product(s) (the "Services").    #
# Except as may be otherwise agreed to in writing, no warranties of any       #
# kind, whether express or implied, are given by MediaTek Inc. with respect   #
# to the Services provided, and the Services are provided on an "AS IS"       #
# basis. Receiver further acknowledges that the Services may contain errors   #
# that testing is important and it is solely responsible for fully testing    #
# the Services and/or derivatives thereof before they are used, sublicensed   #
# or distributed. Should there be any third party action brought against      #
# MediaTek Inc. arising out of or relating to the Services, Receiver agree    #
# to fully indemnify and hold MediaTek Inc. harmless.  If the parties         #
# mutually agree to enter into or continue a business relationship or other   #
# arrangement, the terms and conditions set forth herein shall remain         #
# effective and, unless explicitly stated otherwise, shall prevail in the     #
# event of a conflict in the terms in any agreements entered into between     #
# the parties.                                                                #
#                                                                             #
#   4)Receiver's sole and exclusive remedy and MediaTek Inc.'s entire and     #
# cumulative liability with respect to MediaTek Software released hereunder   #
# will be, at MediaTek Inc.'s sole discretion, to replace or revise the       #
# MediaTek Software at issue.                                                 #
#                                                                             #
#   5)The transaction contemplated hereunder shall be construed in            #
# accordance with the laws of Singapore, excluding its conflict of laws       #
# principles.  Any disputes, controversies or claims arising thereof and      #
# related thereto shall be settled via arbitration in Singapore, under the    #
# then current rules of the International Chamber of Commerce (ICC).  The     #
# arbitration shall be conducted in English. The awards of the arbitration    #
# shall be final and binding upon both parties and shall be entered and       #
# enforceable in any court of competent jurisdiction.                         #
###############################################################################
sub trim($);

# open a file called "sa4a.txt" and assign it 
# a filehandler called INP.
# if file cannot open, execute die().

if ($#ARGV != 0 )
{
	print "usage : parsing mtk_vga.h\n";
	exit;
}
$vga_table = $ARGV[0] ;
#print "$vga_table" ;
@file_tmp= split(/\./, $vga_table);
#print  "$file_tmp[0] $file_tmp[1]\n";
if($file_tmp[1] ne "")
{
	$new_vga_table = sprintf("%s_new.%s", $file_tmp[0], $file_tmp[1]);
	$bak_vga_table = sprintf("%s_old.%s", $file_tmp[0], $file_tmp[1]);
}
else
{
	$new_vga_table = sprintf("%s_new", $file_tmp[0]);
	$bak_vga_table = sprintf("%s_old", $file_tmp[0]);
 
}
#print "$new_file\n" ;

#open(INP, "mtk_vga.h") || die("Cannot open sa4a.txt.\n");
#open(OUTP, ">mtk_vga_new.h") || die "Cannot open file\n";
open(INP, "$vga_table") || die("Cannot open $vga_table.\n");
open(OUTP, ">$new_vga_table") || die "Cannot open $new_vga_table\n";

@recs = <INP>;


# get the first record
$record = $recs[0];

# get rid of consecutive whitespaces
# \s: whitespace, +: one or more.
#@fields = split(/\s+/, $record);
#$num_field = @fields;

# a way of printing what we got.
#print "Record is @fields.\n";
#print "Number of field is $num_field.\n";


# print table header
#print "$record\n";
$start = 0 ;
$idx = 0 ;
# read data till the end of the file
for($row=0; $row < @recs; $row++)
{
	   $record = $recs[$row];
	   if($record =~ /VGATIMING_TABLE\[\]/)
		{
			$start = 1 ;
			#print "fisrt entry $record" ;
		}
	    if($start == 1 )
		{
			if($record !~ /^\s*\/\//) #first char with space // skip 
			{
				if($record =~ /^\s*\/\*/) # with /* start
				{
					#test here	
					$_ = $record ;
					s/^\s*\/\*(.*)\*\/.*/\1/; # record \1 between /* */
					if(($_ =~ /0/) && ($_ == 0))
					{
						$idx = 0 ;
						#	printf("rst idx %d", $_);
					}

					$_ = $record ;
					s/^.*{(.*)}/\1/; # record \1 between { }
					@entry = split(/:/);
					$comment = $entry[1] ;
					$_ = $entry[0];
					@entry= split(/,/);
					#print "$_\n" ;
					
					$combine = hex(trim($entry[10]));
					$record = sprintf("\/\*%.3d\*\/{%d,	%d,	%d,	%d,	%d,	%d,	%d,	%d,	%d,	%d,	0x%.2X},	\/\/ %.3d:%s",
					$idx, 
					trim($entry[0]), trim($entry[1]), trim($entry[2]), trim($entry[3]), 
					trim($entry[4]), trim($entry[5]), trim($entry[6]), trim($entry[7]), 
					trim($entry[8]), trim($entry[9]), $combine,
					$idx, $comment);
					#$record = $_;
					
					$idx++ ;	
					#print "$record\n";
				}

				if($record =~ /};/)
				{
					$start = 0;
					#print "end entry $record" ;
				}
			}
			else
			{
				
			}
		}
		#print "$record\n";
		print OUTP "$record";
}

close INP;
close OUTP;
system "cp $vga_table $bak_vga_table" ;
system "cp $new_vga_table $vga_table" ;
system "rm -f $new_vga_table" ;

sub trim($)
{
		my $string = shift;
		$string =~ s/^\s+//;
		$string =~ s/\s+$//;
		return $string;
}
