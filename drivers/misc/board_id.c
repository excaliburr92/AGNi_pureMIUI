/*
 *
 * Copyright (C) 2019 wanghan <wanghan@longcheer.com>
 * Copyright (C) 2021 XiaoMi, Inc.
 *
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/errno.h>
#include <linux/slab.h>
#include <linux/string.h>

/*
 *
 * String :
 *     'androidboot.hwname='
 * Format :
 *     'androidboot.hwname=<string>'
 * Example :
 *     androidboot.hwname=curtana
 *     androidboot.hwname=excalibur
 *     androidboot.hwname=durandal
 *     androidboot.hwname=joyeuse
 *******************************************************************
 */

static char board_id_hwname[32] = {0};
static bool board_33w_supported = false;
static bool board_nfc_supported = false;

bool board_get_33w_supported(void)
{
	return board_33w_supported;
}

bool board_get_nfc_supported(void)
{
	return board_nfc_supported;
}

void board_id_get_hwname(char *str)
{
	if(IS_ERR_OR_NULL(str))
		return;
	strcpy(str, board_id_hwname);
}
EXPORT_SYMBOL(board_id_get_hwname);

static int __init setup_board_id_hwname(char *str)
{
	strcpy(board_id_hwname, str);
	pr_info("board_id_hwname : %s\n", board_id_hwname);

	if (!strcmp(str, "excalibur")
			|| !strcmp(str, "joyeuse")
			|| !strcmp(str, "gram"))
		board_33w_supported = true;
	else if (!strcmp(str, "curtana"))
		board_33w_supported = false;

	if (!strcmp(str, "joyeuse"))
		board_nfc_supported = true;

	pr_info("board_33w_supported : %s\n",
			board_33w_supported ? "true" : "false");

	return 1;
}
__setup("androidboot.hwname=", setup_board_id_hwname);

/*
 *
 * String :
 *     'androidboot.hwversion='
 * Format :
 *     'androidboot.hwversion=<number/string>'
 * Example :
 *     androidboot.hwlevel=0
 *     androidboot.hwlevel=1
 *     androidboot.hwlevel=2
 *     androidboot.hwlevel=MP (9)
 *******************************************************************
 */

static int board_id_hwlevel;

int board_id_get_hwlevel(void)
{
	return board_id_hwlevel;
}
EXPORT_SYMBOL(board_id_get_hwlevel);

static int __init setup_board_id_hwlevel(char *str)
{
	if (!strcmp("MP", str)) {
		board_id_hwlevel = 9;
		pr_info("board_id_hwlevel : %d (MP)\n", board_id_hwlevel);
		return 1;
	}
	board_id_hwlevel = simple_strtol(str, NULL, 10);
	pr_info("board_id_hwlevel : %d\n", board_id_hwlevel);
	return 1;
}
__setup("androidboot.hwlevel=", setup_board_id_hwlevel);

/*
 *
 * String :
 *     'androidboot.hwversion='
 * Format :
 *     xx.xx.xx < product_number . major_number . minor_number >
 * Example :
 *     androidboot.hwversion=xx.xx.xx // product  , major, minor
 *     androidboot.hwversion=1.13.0   // curtana  , 13   , 0 (CN)
 *     androidboot.hwversion=1.90.0   // curtana  , 90   , 0 (IN)
 *     androidboot.hwversion=1.0.0    // curtana  , 0    , 0
 *     androidboot.hwversion=2.90.0   // excalibur, 90   , 0 (IN)
 *     androidboot.hwversion=3.20.0   // durandal , 20   , 0
 *     androidboot.hwversion=4.90.0   // joyeuse  , 90   , 0
 *     androidboot.hwversion=6.90.0   // gram     , 90   , 0 (IN)
 *******************************************************************
 */

static int board_id_hwversion_product_num;
static int board_id_hwversion_major_num;
static int board_id_hwversion_minor_num;

int board_id_get_hwversion_product_num(void)
{
	return board_id_hwversion_product_num;
}
EXPORT_SYMBOL(board_id_get_hwversion_product_num);

int board_id_get_hwversion_major_num(void)
{
	return board_id_hwversion_major_num;
}
EXPORT_SYMBOL(board_id_get_hwversion_major_num);

int board_id_get_hwversion_minor_num(void)
{
	return board_id_hwversion_minor_num;
}
EXPORT_SYMBOL(board_id_get_hwversion_minor_num);

static int __init setup_board_id_hwversion(char *str)
{
	char *str_p = NULL;
	char *str_n = NULL;
	char buf[32] = {0};
	strcpy(buf, str);
	str_n = buf;
	str_p = strsep(&str_n, ".");
	if (str_p)
		board_id_hwversion_product_num = simple_strtoul(str_p, NULL, 10);
	str_p = strsep(&str_n, ".");
	if (str_p)
		board_id_hwversion_major_num = simple_strtoul(str_p, NULL, 10);
	if (str_n)
		board_id_hwversion_minor_num = simple_strtoul(str_n, NULL, 10);
	pr_info("board_id_hwversion_product_num : %d\n", board_id_hwversion_product_num);
	pr_info("board_id_hwversion_major_num : %d\n", board_id_hwversion_major_num);
	pr_info("board_id_hwversion_minor_num : %d\n", board_id_hwversion_minor_num);
	if ((board_id_hwversion_product_num == 1) && (board_id_hwversion_major_num == 13))
		board_nfc_supported = true;
	else if (board_id_hwversion_product_num == 4)
		board_nfc_supported = true;

	return 1;
}
__setup("androidboot.hwversion=", setup_board_id_hwversion);

