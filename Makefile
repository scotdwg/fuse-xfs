# $NetBSD$

HG_COMMIT=	dea73332be091793f4e9212bfcadf276e7080ea2
HG_BRANCH=	trunk
EXTRACT_SUFX=   .zip
DISTNAME=	fusexfs-${HG_BRANCH}-${HG_COMMIT}

PKGNAME=	fuse-xfs-0.1
PKGREVISION=	1
CATEGORIES=	filesystems

MASTER_SITES=	http://sourceforge.net/code-snapshots/hg/f/fu/fusexfs/${HG_BRANCH}/

MAINTAINER=	pkgsrc@NetBSD.org
HOMEPAGE=	http://www.ahardy.za.net/Home/fusexfs/
COMMENT=	FUSE file-system to mount XFS file systems
LICENSE=	gnu-gpl-v2

ONLY_FOR_PLATFORM=	NetBSD-*-*

GNU_CONFIGURE=	yes
USE_LIBTOOL=	yes
USE_TOOLS+=	autoheader automake autoreconf gmake
USE_LANGUAGES=	c c++

pre-configure:
	cd ${WRKSRC}/src/xfsprogs && gmake configure

pre-install:
	mkdir -p ${DESTDIR}${PREFIX}/bin ${DESTDIR}${PREFIX}/libexec

post-install:
	install -m 755 files/fuse-xfs ${DESTDIR}${PREFIX}/bin
	install -m 755 files/fuse-xfs.probe ${DESTDIR}${PREFIX}/bin

.include "../../mk/fuse.buildlink3.mk"
.include "../../mk/bsd.pkg.mk"
