/* SPDX-License-Identifier: BSD-3-Clause
 * Copyright (c) 2014-2018 Netronome Systems, Inc.
 * All rights reserved.
 */

#ifndef _NFP_COMMON_H_
#define _NFP_COMMON_H_

#include <bus_pci_driver.h>
#include <ethdev_driver.h>
#include <rte_io.h>
#include <rte_spinlock.h>

#include "nfp_ctrl.h"
#include "nfpcore/nfp_dev.h"

/* Macros for accessing the Queue Controller Peripheral 'CSRs' */
#define NFP_QCP_QUEUE_OFF(_x)                 ((_x) * 0x800)
#define NFP_QCP_QUEUE_ADD_RPTR                  0x0000
#define NFP_QCP_QUEUE_ADD_WPTR                  0x0004
#define NFP_QCP_QUEUE_STS_LO                    0x0008
#define NFP_QCP_QUEUE_STS_LO_READPTR_mask     (0x3ffff)
#define NFP_QCP_QUEUE_STS_HI                    0x000c
#define NFP_QCP_QUEUE_STS_HI_WRITEPTR_mask    (0x3ffff)

/* Interrupt definitions */
#define NFP_NET_IRQ_LSC_IDX             0

/* Default values for RX/TX configuration */
#define DEFAULT_RX_FREE_THRESH  32
#define DEFAULT_RX_PTHRESH      8
#define DEFAULT_RX_HTHRESH      8
#define DEFAULT_RX_WTHRESH      0

#define DEFAULT_TX_RS_THRESH	32
#define DEFAULT_TX_FREE_THRESH  32
#define DEFAULT_TX_PTHRESH      32
#define DEFAULT_TX_HTHRESH      0
#define DEFAULT_TX_WTHRESH      0
#define DEFAULT_TX_RSBIT_THRESH 32

/* Alignment for dma zones */
#define NFP_MEMZONE_ALIGN	128

#define NFP_QCP_QUEUE_ADDR_SZ   (0x800)

/* Number of supported physical ports */
#define NFP_MAX_PHYPORTS	12

/* Firmware application ID's */
enum nfp_app_fw_id {
	NFP_APP_FW_CORE_NIC               = 0x1,
	NFP_APP_FW_FLOWER_NIC             = 0x3,
};

/* nfp_qcp_ptr - Read or Write Pointer of a queue */
enum nfp_qcp_ptr {
	NFP_QCP_READ_PTR = 0,
	NFP_QCP_WRITE_PTR
};

enum nfp_net_meta_format {
	NFP_NET_METAFORMAT_SINGLE,
	NFP_NET_METAFORMAT_CHAINED,
};

/* Parsed control BAR TLV capabilities */
struct nfp_net_tlv_caps {
	uint32_t mbox_off;               /**< VNIC mailbox area offset */
	uint32_t mbox_len;               /**< VNIC mailbox area length */
	uint32_t mbox_cmsg_types;        /**< Cmsgs which can be passed through the mailbox */
};

struct nfp_pf_dev {
	/* Backpointer to associated pci device */
	struct rte_pci_device *pci_dev;

	enum nfp_app_fw_id app_fw_id;

	/* Pointer to the app running on the PF */
	void *app_fw_priv;

	/* The eth table reported by firmware */
	struct nfp_eth_table *nfp_eth_table;

	uint8_t *ctrl_bar;

	struct nfp_cpp *cpp;
	struct nfp_cpp_area *ctrl_area;
	struct nfp_cpp_area *qc_area;

	uint8_t *qc_bar;

	struct nfp_hwinfo *hwinfo;
	struct nfp_rtsym_table *sym_tbl;

	/* service id of cpp bridge service */
	uint32_t cpp_bridge_id;
};

struct nfp_app_fw_nic {
	/* Backpointer to the PF device */
	struct nfp_pf_dev *pf_dev;

	/*
	 * Array of physical ports belonging to the this CoreNIC app
	 * This is really a list of vNIC's. One for each physical port
	 */
	struct nfp_net_hw *ports[NFP_MAX_PHYPORTS];

	bool multiport;
	uint8_t total_phyports;
};

struct nfp_net_hw {
	/* Backpointer to the PF this port belongs to */
	struct nfp_pf_dev *pf_dev;

	/* Backpointer to the eth_dev of this port*/
	struct rte_eth_dev *eth_dev;

	/* Info from the firmware */
	struct nfp_net_fw_ver ver;
	uint32_t cap;
	uint32_t max_mtu;
	uint32_t mtu;
	uint32_t rx_offset;
	enum nfp_net_meta_format meta_format;

	/** NFP ASIC params */
	const struct nfp_dev_info *dev_info;

	/* Current values for control */
	uint32_t ctrl;

	uint8_t *ctrl_bar;
	uint8_t *tx_bar;
	uint8_t *rx_bar;

	int stride_rx;
	int stride_tx;

	uint16_t vxlan_ports[NFP_NET_N_VXLAN_PORTS];
	uint8_t vxlan_usecnt[NFP_NET_N_VXLAN_PORTS];

	uint8_t *qcp_cfg;
	rte_spinlock_t reconfig_lock;

	uint32_t max_tx_queues;
	uint32_t max_rx_queues;
	uint16_t flbufsz;
	uint16_t device_id;
	uint16_t vendor_id;
	uint16_t subsystem_device_id;
	uint16_t subsystem_vendor_id;

	struct rte_ether_addr mac_addr;

	/* Records starting point for counters */
	struct rte_eth_stats eth_stats_base;
	struct rte_eth_xstat *eth_xstats_base;

	struct nfp_cpp *cpp;
	struct nfp_cpp_area *ctrl_area;
	struct nfp_cpp_area *mac_stats_area;
	uint8_t *mac_stats_bar;
	uint8_t *mac_stats;

	/* Sequential physical port number, only valid for CoreNIC firmware */
	uint8_t idx;
	/* Internal port number as seen from NFP */
	uint8_t nfp_idx;

	struct nfp_net_tlv_caps tlv_caps;

	struct nfp_net_ipsec_data *ipsec_data;
};

struct nfp_net_adapter {
	struct nfp_net_hw hw;
};

static inline uint8_t nn_readb(volatile const void *addr)
{
	return rte_read8(addr);
}

static inline void nn_writeb(uint8_t val, volatile void *addr)
{
	rte_write8(val, addr);
}

static inline uint32_t nn_readl(volatile const void *addr)
{
	return rte_read32(addr);
}

static inline void nn_writel(uint32_t val, volatile void *addr)
{
	rte_write32(val, addr);
}

static inline uint16_t nn_readw(volatile const void *addr)
{
	return rte_read16(addr);
}

static inline void nn_writew(uint16_t val, volatile void *addr)
{
	rte_write16(val, addr);
}

static inline uint64_t nn_readq(volatile void *addr)
{
	const volatile uint32_t *p = addr;
	uint32_t low, high;

	high = nn_readl((volatile const void *)(p + 1));
	low = nn_readl((volatile const void *)p);

	return low + ((uint64_t)high << 32);
}

static inline void nn_writeq(uint64_t val, volatile void *addr)
{
	nn_writel(val >> 32, (volatile char *)addr + 4);
	nn_writel(val, addr);
}

/*
 * Functions to read/write from/to Config BAR
 * Performs any endian conversion necessary.
 */
static inline uint8_t
nn_cfg_readb(struct nfp_net_hw *hw, int off)
{
	return nn_readb(hw->ctrl_bar + off);
}

static inline void
nn_cfg_writeb(struct nfp_net_hw *hw, int off, uint8_t val)
{
	nn_writeb(val, hw->ctrl_bar + off);
}

static inline uint16_t
nn_cfg_readw(struct nfp_net_hw *hw, int off)
{
	return rte_le_to_cpu_16(nn_readw(hw->ctrl_bar + off));
}

static inline void
nn_cfg_writew(struct nfp_net_hw *hw, int off, uint16_t val)
{
	nn_writew(rte_cpu_to_le_16(val), hw->ctrl_bar + off);
}

static inline uint32_t
nn_cfg_readl(struct nfp_net_hw *hw, int off)
{
	return rte_le_to_cpu_32(nn_readl(hw->ctrl_bar + off));
}

static inline void
nn_cfg_writel(struct nfp_net_hw *hw, int off, uint32_t val)
{
	nn_writel(rte_cpu_to_le_32(val), hw->ctrl_bar + off);
}

static inline uint64_t
nn_cfg_readq(struct nfp_net_hw *hw, int off)
{
	return rte_le_to_cpu_64(nn_readq(hw->ctrl_bar + off));
}

static inline void
nn_cfg_writeq(struct nfp_net_hw *hw, int off, uint64_t val)
{
	nn_writeq(rte_cpu_to_le_64(val), hw->ctrl_bar + off);
}

/*
 * nfp_qcp_ptr_add - Add the value to the selected pointer of a queue
 * @q: Base address for queue structure
 * @ptr: Add to the Read or Write pointer
 * @val: Value to add to the queue pointer
 */
static inline void
nfp_qcp_ptr_add(uint8_t *q, enum nfp_qcp_ptr ptr, uint32_t val)
{
	uint32_t off;

	if (ptr == NFP_QCP_READ_PTR)
		off = NFP_QCP_QUEUE_ADD_RPTR;
	else
		off = NFP_QCP_QUEUE_ADD_WPTR;

	nn_writel(rte_cpu_to_le_32(val), q + off);
}

/*
 * nfp_qcp_read - Read the current Read/Write pointer value for a queue
 * @q:  Base address for queue structure
 * @ptr: Read or Write pointer
 */
static inline uint32_t
nfp_qcp_read(uint8_t *q, enum nfp_qcp_ptr ptr)
{
	uint32_t off;
	uint32_t val;

	if (ptr == NFP_QCP_READ_PTR)
		off = NFP_QCP_QUEUE_STS_LO;
	else
		off = NFP_QCP_QUEUE_STS_HI;

	val = rte_cpu_to_le_32(nn_readl(q + off));

	if (ptr == NFP_QCP_READ_PTR)
		return val & NFP_QCP_QUEUE_STS_LO_READPTR_mask;
	else
		return val & NFP_QCP_QUEUE_STS_HI_WRITEPTR_mask;
}

static inline uint32_t
nfp_qcp_queue_offset(const struct nfp_dev_info *dev_info,
		uint16_t queue)
{
	return dev_info->qc_addr_offset + NFP_QCP_QUEUE_ADDR_SZ *
			(queue & dev_info->qc_idx_mask);
}

/* Prototypes for common NFP functions */
int nfp_net_reconfig(struct nfp_net_hw *hw, uint32_t ctrl, uint32_t update);
int nfp_net_ext_reconfig(struct nfp_net_hw *hw, uint32_t ctrl_ext, uint32_t update);
int nfp_net_mbox_reconfig(struct nfp_net_hw *hw, uint32_t mbox_cmd);
int nfp_net_configure(struct rte_eth_dev *dev);
int nfp_net_common_init(struct rte_pci_device *pci_dev, struct nfp_net_hw *hw);
void nfp_net_log_device_information(const struct nfp_net_hw *hw);
void nfp_net_enable_queues(struct rte_eth_dev *dev);
void nfp_net_disable_queues(struct rte_eth_dev *dev);
void nfp_net_params_setup(struct nfp_net_hw *hw);
void nfp_net_write_mac(struct nfp_net_hw *hw, uint8_t *mac);
int nfp_net_set_mac_addr(struct rte_eth_dev *dev, struct rte_ether_addr *mac_addr);
int nfp_configure_rx_interrupt(struct rte_eth_dev *dev,
			       struct rte_intr_handle *intr_handle);
uint32_t nfp_check_offloads(struct rte_eth_dev *dev);
int nfp_net_promisc_enable(struct rte_eth_dev *dev);
int nfp_net_promisc_disable(struct rte_eth_dev *dev);
int nfp_net_link_update(struct rte_eth_dev *dev,
			__rte_unused int wait_to_complete);
int nfp_net_stats_get(struct rte_eth_dev *dev, struct rte_eth_stats *stats);
int nfp_net_stats_reset(struct rte_eth_dev *dev);
uint32_t nfp_net_xstats_size(const struct rte_eth_dev *dev);
int nfp_net_xstats_get_names(struct rte_eth_dev *dev,
		struct rte_eth_xstat_name *xstats_names,
		unsigned int size __rte_unused);
int nfp_net_xstats_get(struct rte_eth_dev *dev,
		struct rte_eth_xstat *xstats,
		unsigned int n __rte_unused);
int nfp_net_xstats_get_names_by_id(struct rte_eth_dev *dev,
		const uint64_t *ids,
		struct rte_eth_xstat_name *xstats_names,
		unsigned int size);
int nfp_net_xstats_get_by_id(struct rte_eth_dev *dev,
		const uint64_t *ids,
		uint64_t *values,
		unsigned int n);
int nfp_net_xstats_reset(struct rte_eth_dev *dev);
int nfp_net_infos_get(struct rte_eth_dev *dev,
		      struct rte_eth_dev_info *dev_info);
const uint32_t *nfp_net_supported_ptypes_get(struct rte_eth_dev *dev);
int nfp_rx_queue_intr_enable(struct rte_eth_dev *dev, uint16_t queue_id);
int nfp_rx_queue_intr_disable(struct rte_eth_dev *dev, uint16_t queue_id);
void nfp_net_params_setup(struct nfp_net_hw *hw);
void nfp_net_cfg_queue_setup(struct nfp_net_hw *hw);
void nfp_net_dev_interrupt_handler(void *param);
void nfp_net_dev_interrupt_delayed_handler(void *param);
int nfp_net_dev_mtu_set(struct rte_eth_dev *dev, uint16_t mtu);
int nfp_net_vlan_offload_set(struct rte_eth_dev *dev, int mask);
int nfp_net_reta_update(struct rte_eth_dev *dev,
			struct rte_eth_rss_reta_entry64 *reta_conf,
			uint16_t reta_size);
int nfp_net_reta_query(struct rte_eth_dev *dev,
		       struct rte_eth_rss_reta_entry64 *reta_conf,
		       uint16_t reta_size);
int nfp_net_rss_hash_update(struct rte_eth_dev *dev,
			    struct rte_eth_rss_conf *rss_conf);
int nfp_net_rss_hash_conf_get(struct rte_eth_dev *dev,
			      struct rte_eth_rss_conf *rss_conf);
int nfp_net_rss_config_default(struct rte_eth_dev *dev);
void nfp_net_stop_rx_queue(struct rte_eth_dev *dev);
void nfp_net_close_rx_queue(struct rte_eth_dev *dev);
void nfp_net_stop_tx_queue(struct rte_eth_dev *dev);
void nfp_net_close_tx_queue(struct rte_eth_dev *dev);
int nfp_net_set_vxlan_port(struct nfp_net_hw *hw, size_t idx, uint16_t port);
void nfp_net_rx_desc_limits(struct nfp_net_hw *hw,
		uint16_t *min_rx_desc,
		uint16_t *max_rx_desc);
void nfp_net_tx_desc_limits(struct nfp_net_hw *hw,
		uint16_t *min_tx_desc,
		uint16_t *max_tx_desc);
int nfp_net_check_dma_mask(struct nfp_net_hw *hw, char *name);
void nfp_net_init_metadata_format(struct nfp_net_hw *hw);
void nfp_net_cfg_read_version(struct nfp_net_hw *hw);
int nfp_net_firmware_version_get(struct rte_eth_dev *dev, char *fw_version, size_t fw_size);
int nfp_repr_firmware_version_get(struct rte_eth_dev *dev, char *fw_version, size_t fw_size);
bool nfp_net_is_valid_nfd_version(struct nfp_net_fw_ver version);

#define NFP_NET_DEV_PRIVATE_TO_HW(adapter)\
	(&((struct nfp_net_adapter *)adapter)->hw)

#define NFP_NET_DEV_PRIVATE_TO_PF(dev_priv)\
	(((struct nfp_net_hw *)dev_priv)->pf_dev)

#define NFP_PRIV_TO_APP_FW_NIC(app_fw_priv)\
	((struct nfp_app_fw_nic *)app_fw_priv)

#define NFP_PRIV_TO_APP_FW_FLOWER(app_fw_priv)\
	((struct nfp_app_fw_flower *)app_fw_priv)

#endif /* _NFP_COMMON_H_ */
