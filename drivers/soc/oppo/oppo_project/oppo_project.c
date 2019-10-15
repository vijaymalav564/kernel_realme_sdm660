
#include <asm/uaccess.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/fs.h>
#include <soc/qcom/smem.h>
#include <soc/oppo/oppo_project.h>
/*#include <mach/oppo_reserve3.h>*/
#include <linux/io.h>

/*****************************************************/
static struct proc_dir_entry *oppoVersion = NULL;
static ProjectInfoCDTType *format = NULL;

void init_project_version(void)
{
        unsigned int len = (sizeof(ProjectInfoCDTType) + 3)&(~0x3);

        format = (ProjectInfoCDTType *)smem_alloc(SMEM_PROJECT, len, 0, 0);
        if (format == ERR_PTR(-EPROBE_DEFER)) {
                format = NULL;
        }
}


unsigned int get_project(void)
{
        if (format == NULL) {
                init_project_version();
        }
        return format->nproject;
}

unsigned char get_PCB_Version(void)
{
        if (format == NULL) {
                init_project_version();
        }
        return format->npcbversion;
}

unsigned char get_Modem_Version(void)
{
        if (format == NULL) {
                init_project_version();
        }
        return format->nmodem;
}

unsigned char get_Operator_Version(void)
{
        if (format == NULL) {
                init_project_version();
        }
        return  format->noperator;
}


unsigned char get_Oppo_Boot_Mode(void)
{
        if (format == NULL) {
                init_project_version();
        }
        return  format->noppobootmode;
}

/*this module just init for creat files to show which version*/
static ssize_t prjVersion_read_proc(struct file *file, char __user *buf,
                size_t count, loff_t *off)
{
        char page[256] = {0};
        int len = 0;
        len = sprintf(page, "%d", get_project());

        if (len > *off) {
                len -= *off;
        }
        else
                len = 0;
        if (copy_to_user(buf, page, (len < count ? len : count))) {
                return -EFAULT;
        }
        *off += len < count ? len : count;
        return (len < count ? len : count);
}

struct file_operations prjVersion_proc_fops = {
        .read = prjVersion_read_proc,
        .write = NULL,
};


static ssize_t pcbVersion_read_proc(struct file *file, char __user *buf,
                size_t count, loff_t *off)
{
        char page[256] = {0};
        int len = 0;

        len = sprintf(page, "%d", get_PCB_Version());

        if (len > *off) {
                len -= *off;
        }
        else
                len = 0;

        if (copy_to_user(buf, page, (len < count ? len : count))) {
                return -EFAULT;
        }
        *off += len < count ? len : count;
        return (len < count ? len : count);
}

struct file_operations pcbVersion_proc_fops = {
        .read = pcbVersion_read_proc,
};


static ssize_t operatorName_read_proc(struct file *file, char __user *buf,
                size_t count, loff_t *off)
{
        char page[256] = {0};
        int len = 0;

        len = sprintf(page, "%d", get_Operator_Version());

        if (len > *off) {
                len -= *off;
        }
        else
                len = 0;

        if (copy_to_user(buf, page, (len < count ? len : count))) {
                return -EFAULT;
        }
        *off += len < count ? len : count;
        return (len < count ? len : count);
}

struct file_operations operatorName_proc_fops = {
        .read = operatorName_read_proc,
};


static ssize_t modemType_read_proc(struct file *file, char __user *buf,
                size_t count, loff_t *off)
{
        char page[256] = {0};
        int len = 0;

        len = sprintf(page, "%d", get_Modem_Version());

        if (len > *off) {
                len -= *off;
        }
        else
                len = 0;

        if (copy_to_user(buf, page, (len < count ? len : count))) {
                return -EFAULT;
        }
        *off += len < count ? len : count;
        return (len < count ? len : count);
}

struct file_operations modemType_proc_fops = {
        .read = modemType_read_proc,
};


static ssize_t oppoBootmode_read_proc(struct file *file, char __user *buf,
                size_t count, loff_t *off)
{
        char page[256] = {0};
        int len = 0;

        len = sprintf(page, "%d", get_Oppo_Boot_Mode());

        if (len > *off) {
                len -= *off;
        }
        else
                len = 0;

        if (copy_to_user(buf, page, (len < count ? len : count))) {
                return -EFAULT;
        }
        *off += len < count ? len : count;
        return (len < count ? len : count);
}

struct file_operations oppoBootmode_proc_fops = {
        .read = oppoBootmode_read_proc,
};

#ifdef CONFIG_PRODUCT_REALME_RMX1801
/*Ziqing.Guo@BSP.Fingerprint.Secure 2017/03/28 Add for displaying secure boot switch*/
#define OEM_SEC_BOOT_REG 0x780350 /*sdm660
*/
static ssize_t secureType_read_proc(struct file *file, char __user *buf,
                size_t count, loff_t *off)
{
        char page[256] = {0};
        int len = 0;
        void __iomem *oem_config_base;
        uint32_t secure_oem_config = 0;

        oem_config_base = ioremap(OEM_SEC_BOOT_REG, 4);
        secure_oem_config = __raw_readl(oem_config_base);
        iounmap(oem_config_base);
        printk(KERN_EMERG "lycan test secure_oem_config 0x%x\n", secure_oem_config);
        len = sprintf(page, "%d", secure_oem_config);

        if (len > *off) {
                len -= *off;
        }
        else
                len = 0;

        if (copy_to_user(buf, page, (len < count ? len : count))) {
                return -EFAULT;
        }
        *off += len < count ? len : count;
        return (len < count ? len : count);
}
#endif /* CONFIG_PRODUCT_REALME_RMX1801 */

#ifdef CONFIG_PRODUCT_REALME_RMX1801
/*Ziqing.Guo@BSP.Fingerprint.Secure 2017/04/16 Add for distinguishing secureboot stage*/
#define OEM_SEC_ENABLE_ANTIROLLBACK_REG 0x78019c /*sdm660
*/
static ssize_t secureStage_read_proc(struct file *file, char __user *buf,
                size_t count, loff_t *off)
{
        char page[256] = {0};
        int len = 0;
        void __iomem *oem_config_base;
        uint32_t secure_oem_config = 0;

        oem_config_base = ioremap(OEM_SEC_ENABLE_ANTIROLLBACK_REG, 4);
        secure_oem_config = __raw_readl(oem_config_base);
        iounmap(oem_config_base);
        printk(KERN_EMERG "lycan test secureStage_oem_config 0x%x\n", secure_oem_config);
        len = sprintf(page, "%d", secure_oem_config);

        if (len > *off) {
                len -= *off;
        }
        else
                len = 0;

        if (copy_to_user(buf, page, (len < count ? len : count))) {
                return -EFAULT;
        }
        *off += len < count ? len : count;
        return (len < count ? len : count);
}
#endif /* CONFIG_PRODUCT_REALME_RMX1801 */

struct file_operations secureType_proc_fops = {
        .read = secureType_read_proc,
};

#ifdef CONFIG_PRODUCT_REALME_RMX1801
/*Ziqing.Guo@BSP.Fingerprint.Secure 2017/04/16 Add for distinguishing secureboot stage*/
struct file_operations secureStage_proc_fops = {
        .read = secureStage_read_proc,
};
#endif /* CONFIG_PRODUCT_REALME_RMX1801 */

#define QFPROM_RAW_SERIAL_NUM 0x00786134 /*different at each platform, please ref boot_images\core\systemdrivers\hwio\scripts\xxx\hwioreg.per
*/

static unsigned int g_serial_id = 0x00; /*maybe can use for debug
*/

static ssize_t serialID_read_proc(struct file *file, char __user *buf,
                size_t count, loff_t *off)
{
        char page[256] = {0};
        int len = 0;
        len = sprintf(page, "0x%x", g_serial_id);

        if (len > *off) {
                len -= *off;
        }
        else
                len = 0;

        if (copy_to_user(buf, page, (len < count ? len : count))) {
                return -EFAULT;
        }
        *off += len < count ? len : count;
        return (len < count ? len : count);
}


struct file_operations serialID_proc_fops = {
        .read = serialID_read_proc,
};
/*for get which ldo ocp*/
void print_ocp(void)
{
        int i = 0;

        if (format == NULL) {
                init_project_version();
        }
        printk("ocp:");
        for (i = 0;i < OCPCOUNTMAX;i++) {
                printk(" %d", format->npmicocp[i]);
        }
        printk("\n");
}

static int __init ocplog_init(void)
{
        print_ocp();
        return 0;
}
late_initcall(ocplog_init);
static ssize_t ocplog_read_proc(struct file *file, char __user *buf,
                size_t count, loff_t *off)
{
        char page[256] = {0};
        int len = 0;
        int i = 0;

        if (format == NULL) {
                init_project_version();
        }
        len += sprintf(&page[len], "ocp:");
        for (i = 0;i < OCPCOUNTMAX;i++) {
                len += sprintf(&page[len], " %d", format->npmicocp[i]);
        }
        len += sprintf(&page[len], "\n");

        if (len > *off) {
                len -= *off;
        }
        else
                len = 0;

        if (copy_to_user(buf, page, (len < count ? len : count))) {
                return -EFAULT;
        }
        *off += len < count ? len : count;
        return (len < count ? len : count);
}


struct file_operations ocp_proc_fops = {
        .read = ocplog_read_proc,
};


#ifdef CONFIG_PRODUCT_REALME_RMX1801
/* ChaoYing.Chen@BSP.Kernel.Boot, 2018/11/06, Add for add cmdline for exp_opera info*/
#define ITEM_LENGTH 50
char expOpera[ITEM_LENGTH + 1];
static ssize_t expOpera_proc_read(struct file *file, char __user *buf, size_t count, loff_t *off)
{
	char page[256] = {0};
	int len = 0;

	if(expOpera[0] == 0)
		return 0;
	len = sprintf(page,"%s",expOpera);

	if(len > *off)
		len -= *off;
	else
		len = 0;

	if(copy_to_user(buf, page,(len < count ? len : count))){
		return -EFAULT;
	}
	*off += len < count ? len : count;
	return (len < count ? len : count);
}

static const struct file_operations expOpera_fops =
{
    .write = NULL,
    .read  = expOpera_proc_read,
    .owner = THIS_MODULE,
};
#endif

static int __init oppo_project_init(void)
{
        int ret = 0;
        struct proc_dir_entry *pentry;
        void __iomem *serial_id_addr = NULL;

        serial_id_addr = ioremap(QFPROM_RAW_SERIAL_NUM , 4);
        if (serial_id_addr) {
                g_serial_id = __raw_readl(serial_id_addr);
                iounmap(serial_id_addr);
                printk(KERN_EMERG "serialID 0x%x\n", g_serial_id);
        } else
        {
                g_serial_id = 0xffffffff;
        }
        oppoVersion =  proc_mkdir("oppoVersion", NULL);
        if (!oppoVersion) {
                pr_err("can't create oppoVersion proc\n");
                goto ERROR_INIT_VERSION;
        }
        pentry = proc_create("prjVersion", S_IRUGO, oppoVersion, &prjVersion_proc_fops);
        if (!pentry) {
                pr_err("create prjVersion proc failed.\n");
                goto ERROR_INIT_VERSION;
        }
        pentry = proc_create("pcbVersion", S_IRUGO, oppoVersion, &pcbVersion_proc_fops);
        if (!pentry) {
                pr_err("create pcbVersion proc failed.\n");
                goto ERROR_INIT_VERSION;
        }
        pentry = proc_create("operatorName", S_IRUGO, oppoVersion, &operatorName_proc_fops);
        if (!pentry) {
                pr_err("create operatorName proc failed.\n");
                goto ERROR_INIT_VERSION;
        }
        pentry = proc_create("modemType", S_IRUGO, oppoVersion, &modemType_proc_fops);
        if (!pentry) {
                pr_err("create modemType proc failed.\n");
                goto ERROR_INIT_VERSION;
        }
        pentry = proc_create("oppoBootmode", S_IRUGO, oppoVersion, &oppoBootmode_proc_fops);
        if (!pentry) {
                pr_err("create oppoBootmode proc failed.\n");
                goto ERROR_INIT_VERSION;
        }
        pentry = proc_create("secureType", S_IRUGO, oppoVersion, &secureType_proc_fops);
        if (!pentry) {
                pr_err("create secureType proc failed.\n");
                goto ERROR_INIT_VERSION;
        }

#ifdef CONFIG_PRODUCT_REALME_RMX1801
/*Ziqing.Guo@BSP.Fingerprint.Secure 2017/04/16 Add for distinguishing secureboot stage*/
        pentry = proc_create("secureStage", S_IRUGO, oppoVersion, &secureStage_proc_fops);
        if (!pentry) {
                pr_err("create secureStage proc failed.\n");
                goto ERROR_INIT_VERSION;
        }
#endif /* CONFIG_PRODUCT_REALME_RMX1801 */
        pentry = proc_create("serialID", S_IRUGO, oppoVersion, &serialID_proc_fops);
        if (!pentry) {
                pr_err("create serialID proc failed.\n");
                goto ERROR_INIT_VERSION;
        }

        pentry = proc_create("ocp", S_IRUGO, oppoVersion, &ocp_proc_fops);
        if (!pentry) {
                pr_err("create serialID proc failed.\n");
                goto ERROR_INIT_VERSION;
        }

		#ifdef CONFIG_PRODUCT_REALME_RMX1801
		/* ChaoYing.Chen@BSP.Kernel.Boot, 2018/11/06, Add for add cmdline for exp_opera info*/
		pentry = proc_create("expOpera", 0444, oppoVersion, &expOpera_fops);
		#endif

        return ret;
ERROR_INIT_VERSION:
                remove_proc_entry("oppoVersion", NULL);
                return -ENOENT;
}
arch_initcall(oppo_project_init);

#ifdef CONFIG_PRODUCT_REALME_RMX1801
/* ChaoYing.Chen@BSP.Kernel.Boot, 2018/11/06, Add for add cmdline for exp_opera info*/
static int __init expOpera_init(void)
{
    int i;
    char *substr = strstr(boot_command_line, "exp_operator=");

    if(NULL == substr)
        return 0;

    substr += strlen("exp_operator=");

    for(i=0; substr[i] != ' ' && i < ITEM_LENGTH && substr[i] != '\0'; i++) {
        expOpera[i] = substr[i];
    }
    expOpera[i] = '\0';

    printk(KERN_INFO "%s: parse expOpera is %s\n", __func__, expOpera);
    return 1;
}
arch_initcall(expOpera_init);
#endif

MODULE_DESCRIPTION("OPPO project version");
MODULE_LICENSE("GPL v2");
MODULE_AUTHOR("Joshua <gyx@oppo.com>");
