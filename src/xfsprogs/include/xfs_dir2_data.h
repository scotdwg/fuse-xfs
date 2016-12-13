/*
 * Copyright (c) 2000,2005 Silicon Graphics, Inc.
 * All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it would be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write the Free Software Foundation,
 * Inc.,  51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */
#ifndef __XFS_DIR2_DATA_H__
#define	__XFS_DIR2_DATA_H__

/*
 * Directory format 2, data block structures.
 */

struct xfs_dabuf;
struct xfs_da_args;
struct xfs_inode;
struct xfs_trans;

/*
 * Constants.
 */
#define	XFS_DIR2_DATA_MAGIC	0x58443244	/* XD2D: for multiblock dirs */
#define	XFS_DIR2_DATA_ALIGN_LOG	3		/* i.e., 8 bytes */
#define	XFS_DIR2_DATA_ALIGN	(1 << XFS_DIR2_DATA_ALIGN_LOG)
#define	XFS_DIR2_DATA_FREE_TAG	0xffff
#define	XFS_DIR2_DATA_FD_COUNT	3

/*
 * Directory address space divided into sections,
 * spaces separated by 32gb.
 */
#define	XFS_DIR2_SPACE_SIZE	(1ULL << (32 + XFS_DIR2_DATA_ALIGN_LOG))
#define	XFS_DIR2_DATA_SPACE	0
#define	XFS_DIR2_DATA_OFFSET	(XFS_DIR2_DATA_SPACE * XFS_DIR2_SPACE_SIZE)
#define	XFS_DIR2_DATA_FIRSTDB(mp)	\
	xfs_dir2_byte_to_db(mp, XFS_DIR2_DATA_OFFSET)

/*
 * Offsets of . and .. in data space (always block 0)
 */
#define	XFS_DIR2_DATA_DOT_OFFSET	\
	((xfs_dir2_data_aoff_t)sizeof(xfs_dir2_data_hdr_t))
#define	XFS_DIR2_DATA_DOTDOT_OFFSET	\
	(XFS_DIR2_DATA_DOT_OFFSET + xfs_dir2_data_entsize(1))
#define	XFS_DIR2_DATA_FIRST_OFFSET		\
	(XFS_DIR2_DATA_DOTDOT_OFFSET + xfs_dir2_data_entsize(2))

/*
 * Structures.
 */

/*
 * Describe a free area in the data block.
 * The freespace will be formatted as a xfs_dir2_data_unused_t.
 */
typedef struct xfs_dir2_data_free {
	__be16			offset;		/* start of freespace */
	__be16			length;		/* length of freespace */
} xfs_dir2_data_free_t;

/*
 * Header for the data blocks.
 * Always at the beginning of a directory-sized block.
 * The code knows that XFS_DIR2_DATA_FD_COUNT is 3.
 */
typedef struct xfs_dir2_data_hdr {
	__be32			magic;		/* XFS_DIR2_DATA_MAGIC */
						/* or XFS_DIR2_BLOCK_MAGIC */
	xfs_dir2_data_free_t	bestfree[XFS_DIR2_DATA_FD_COUNT];
} xfs_dir2_data_hdr_t;

/*
 * Active entry in a data block.  Aligned to 8 bytes.
 * Tag appears as the last 2 bytes.
 */
typedef struct xfs_dir2_data_entry {
	__be64			inumber;	/* inode number */
	__u8			namelen;	/* name length */
	__u8			name[1];	/* name bytes, no null */
						/* variable offset */
	__be16			tag;		/* starting offset of us */
} xfs_dir2_data_entry_t;

/*
 * Unused entry in a data block.  Aligned to 8 bytes.
 * Tag appears as the last 2 bytes.
 */
typedef struct xfs_dir2_data_unused {
	__be16			freetag;	/* XFS_DIR2_DATA_FREE_TAG */
	__be16			length;		/* total free length */
						/* variable offset */
	__be16			tag;		/* starting offset of us */
} xfs_dir2_data_unused_t;

typedef union {
	xfs_dir2_data_entry_t	entry;
	xfs_dir2_data_unused_t	unused;
} xfs_dir2_data_union_t;

/*
 * Generic data block structure, for xfs_db.
 */
typedef struct xfs_dir2_data {
	xfs_dir2_data_hdr_t	hdr;		/* magic XFS_DIR2_DATA_MAGIC */
	xfs_dir2_data_union_t	u[1];
} xfs_dir2_data_t;

/*
 * Directory Version 3 With CRCs.
 *
 * The tree formats are the same as for version 2 directories.  The difference
 * is in the block header and dirent formats. In many cases the v3 structures
 * use v2 definitions as they are no different and this makes code sharing much
 * easier.
 *
 * Also, the xfs_dir3_*() functions handle both v2 and v3 formats - if the
 * format is v2 then they switch to the existing v2 code, or the format is v3
 * they implement the v3 functionality. This means the existing dir2 is a mix of
 * xfs_dir2/xfs_dir3 calls and functions. The xfs_dir3 functions are called
 * where there is a difference in the formats, otherwise the code is unchanged.
 *
 * Where it is possible, the code decides what to do based on the magic numbers
 * in the blocks rather than feature bits in the superblock. This means the code
 * is as independent of the external XFS code as possible as doesn't require
 * passing struct xfs_mount pointers into places where it isn't really
 * necessary.
 *
 * Version 3 includes:
 *
 *	- a larger block header for CRC and identification purposes and so the
 *	offsets of all the structures inside the blocks are different.
 *
 *	- new magic numbers to be able to detect the v2/v3 types on the fly.
 */

#define	XFS_DIR3_BLOCK_MAGIC	0x58444233	/* XDB3: single block dirs */
#define	XFS_DIR3_DATA_MAGIC	0x58444433	/* XDD3: multiblock dirs */
#define	XFS_DIR3_FREE_MAGIC	0x58444633	/* XDF3: free index blocks */
/*
 * Data block structures.
 *
 * A pure data block looks like the following drawing on disk:
 *
 *    +-------------------------------------------------+
 *    | xfs_dir2_data_hdr_t                             |
 *    +-------------------------------------------------+
 *    | xfs_dir2_data_entry_t OR xfs_dir2_data_unused_t |
 *    | xfs_dir2_data_entry_t OR xfs_dir2_data_unused_t |
 *    | xfs_dir2_data_entry_t OR xfs_dir2_data_unused_t |
 *    | ...                                             |
 *    +-------------------------------------------------+
 *    | unused space                                    |
 *    +-------------------------------------------------+
 *
 * As all the entries are variable size structures the accessors below should
 * be used to iterate over them.
 *
 * In addition to the pure data blocks for the data and node formats,
 * most structures are also used for the combined data/freespace "block"
 * format below.
 */

#define	XFS_DIR2_DATA_ALIGN_LOG	3		/* i.e., 8 bytes */
#define	XFS_DIR2_DATA_ALIGN	(1 << XFS_DIR2_DATA_ALIGN_LOG)
#define	XFS_DIR2_DATA_FREE_TAG	0xffff
#define	XFS_DIR2_DATA_FD_COUNT	3

/*
 * Directory address space divided into sections,
 * spaces separated by 32GB.
 */
#define	XFS_DIR2_SPACE_SIZE	(1ULL << (32 + XFS_DIR2_DATA_ALIGN_LOG))
#define	XFS_DIR2_DATA_SPACE	0
#define	XFS_DIR2_DATA_OFFSET	(XFS_DIR2_DATA_SPACE * XFS_DIR2_SPACE_SIZE)

/*
 * Dirents in version 3 directories have a file type field. Additions to this
 * list are an on-disk format change, requiring feature bits. Valid values
 * are as follows:
 */
#define XFS_DIR3_FT_UNKNOWN		0
#define XFS_DIR3_FT_REG_FILE		1
#define XFS_DIR3_FT_DIR			2
#define XFS_DIR3_FT_CHRDEV		3
#define XFS_DIR3_FT_BLKDEV		4
#define XFS_DIR3_FT_FIFO		5
#define XFS_DIR3_FT_SOCK		6
#define XFS_DIR3_FT_SYMLINK		7
#define XFS_DIR3_FT_WHT			8

#define XFS_DIR3_FT_MAX			9

/*
 * define a structure for all the verification fields we are adding to the
 * directory block structures. This will be used in several structures.
 * The magic number must be the first entry to align with all the dir2
 * structures so we determine how to decode them just by the magic number.
 */
struct xfs_dir3_blk_hdr {
	__be32			magic;	/* magic number */
	__be32			crc;	/* CRC of block */
	__be64			blkno;	/* first block of the buffer */
	__be64			lsn;	/* sequence number of last write */
	uuid_t			uuid;	/* filesystem we belong to */
	__be64			owner;	/* inode that owns the block */
};

struct xfs_dir3_data_hdr {
	struct xfs_dir3_blk_hdr	hdr;
	xfs_dir2_data_free_t	best_free[XFS_DIR2_DATA_FD_COUNT];
	__be32			pad;	/* 64 bit alignment */
};

#define XFS_DIR3_DATA_CRC_OFF  offsetof(struct xfs_dir3_data_hdr, hdr.crc)

/*
 * Macros.
 */

/*
 * Size of a data entry.
 */
static inline int xfs_dir2_data_entsize(int n)
{
	return (int)roundup(offsetof(xfs_dir2_data_entry_t, name[0]) + (n) + \
		 (uint)sizeof(xfs_dir2_data_off_t), XFS_DIR2_DATA_ALIGN);
}

/*
 * This looks more complex than it should be. But we need to
 * get the type for the ~ right in round_down (it needs to be
 * as wide as the result!), and we want to evaluate the macro
 * arguments just once each.
 */
#define __round_mask(x, y) ((__typeof__(x))((y)-1))
#define round_up(x, y) ((((x)-1) | __round_mask(x, y))+1)
#define round_down(x, y) ((x) & ~__round_mask(x, y))

#define XFS_DIR3_DATA_ENTSIZE(n)                                        \
        round_up((offsetof(struct xfs_dir2_data_entry, name[0]) + (n) + \
                 sizeof(xfs_dir2_data_off_t) + sizeof(__uint8_t)),      \
                XFS_DIR2_DATA_ALIGN)

static inline int xfs_dir3_data_entsize(int n)
{
        return XFS_DIR3_DATA_ENTSIZE(n);
}

static inline struct xfs_dir2_data_entry *
xfs_dir2_data_entry_p(struct xfs_dir2_data_hdr *hdr)
{
        return (struct xfs_dir2_data_entry *)
                ((char *)hdr + sizeof(struct xfs_dir2_data_hdr));
}

static inline struct xfs_dir2_data_entry *
xfs_dir3_data_entry_p(struct xfs_dir2_data_hdr *hdr)
{
        return (struct xfs_dir2_data_entry *)
                ((char *)hdr + sizeof(struct xfs_dir3_data_hdr));
}

/*
 * Pointer to an entry's tag word.
 */
static inline __be16 *
xfs_dir2_data_entry_tag_p(xfs_dir2_data_entry_t *dep)
{
	return (__be16 *)((char *)dep +
		xfs_dir2_data_entsize(dep->namelen) - sizeof(__be16));
}

/*
 * Pointer to a freespace's tag word.
 */
static inline __be16 *
xfs_dir2_data_unused_tag_p(xfs_dir2_data_unused_t *dup)
{
	return (__be16 *)((char *)dup +
			be16_to_cpu(dup->length) - sizeof(__be16));
}

/*
 * Function declarations.
 */
#ifdef DEBUG
extern void xfs_dir2_data_check(struct xfs_inode *dp, struct xfs_dabuf *bp);
#else
#define	xfs_dir2_data_check(dp,bp)
#endif
extern xfs_dir2_data_free_t *xfs_dir2_data_freefind(xfs_dir2_data_t *d,
				xfs_dir2_data_unused_t *dup);
extern xfs_dir2_data_free_t *xfs_dir2_data_freeinsert(xfs_dir2_data_t *d,
				xfs_dir2_data_unused_t *dup, int *loghead);
extern void xfs_dir2_data_freescan(struct xfs_mount *mp, xfs_dir2_data_t *d,
				int *loghead);
extern int xfs_dir2_data_init(struct xfs_da_args *args, xfs_dir2_db_t blkno,
				struct xfs_dabuf **bpp);
extern void xfs_dir2_data_log_entry(struct xfs_trans *tp, struct xfs_dabuf *bp,
				xfs_dir2_data_entry_t *dep);
extern void xfs_dir2_data_log_header(struct xfs_trans *tp,
				struct xfs_dabuf *bp);
extern void xfs_dir2_data_log_unused(struct xfs_trans *tp, struct xfs_dabuf *bp,
				xfs_dir2_data_unused_t *dup);
extern void xfs_dir2_data_make_free(struct xfs_trans *tp, struct xfs_dabuf *bp,
				xfs_dir2_data_aoff_t offset,
				xfs_dir2_data_aoff_t len, int *needlogp,
				int *needscanp);
extern void xfs_dir2_data_use_free(struct xfs_trans *tp, struct xfs_dabuf *bp,
			       xfs_dir2_data_unused_t *dup,
			       xfs_dir2_data_aoff_t offset,
			       xfs_dir2_data_aoff_t len, int *needlogp,
			       int *needscanp);

#endif	/* __XFS_DIR2_DATA_H__ */
