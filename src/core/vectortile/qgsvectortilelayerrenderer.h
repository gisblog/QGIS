/***************************************************************************
  qgsvectortilelayerrenderer.h
  --------------------------------------
  Date                 : March 2020
  Copyright            : (C) 2020 by Martin Dobias
  Email                : wonder dot sk at gmail dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QGSVECTORTILELAYERRENDERER_H
#define QGSVECTORTILELAYERRENDERER_H

#define SIP_NO_FILE

#include "qgsmaplayerrenderer.h"

class QgsVectorTileLayer;
class QgsVectorTileRawData;

#include "qgsvectortilerenderer.h"

/**
 * \ingroup core
 * This class provides map rendering functionality for vector tile layers.
 * In render() function (assumed to be run in a worker thread) it will:
 * 1. fetch vector tiles using QgsVectorTileLoader
 * 2. decode raw tiles into QgsFeature objects using QgsVectorTileDecoder
 * 3. render tiles using a class derived from QgsVectorTileRenderer
 *
 * \since QGIS 3.14
 */
class QgsVectorTileLayerRenderer : public QgsMapLayerRenderer
{
  public:
    //! Creates the renderer. Always called from main thread, should copy whatever necessary from the layer
    QgsVectorTileLayerRenderer( QgsVectorTileLayer *layer, QgsRenderContext &context );

    virtual bool render() override;
    virtual QgsFeedback *feedback() const override { return mFeedback.get(); }

  private:
    void decodeAndDrawTile( const QgsVectorTileRawData &rawTile );

    // data coming from the vector tile layer

    //! Type of the source from which we will be loading tiles (e.g. "xyz" or "mbtiles")
    QString mSourceType;
    //! Path/URL of the source. Format depends on source type
    QString mSourcePath;
    //! Minimum zoom level at which source has any valid tiles (negative = unconstrained)
    int mSourceMinZoom = -1;
    //! Maximum zoom level at which source has any valid tiles (negative = unconstrained)
    int mSourceMaxZoom = -1;
    //! Tile renderer object to do rendering of individual tiles
    std::unique_ptr<QgsVectorTileRenderer> mRenderer;

    //! Whether to draw boundaries of tiles (useful for debugging)
    bool mDrawTileBoundaries = false;

    // temporary data used during rendering process

    //! Feedback object that may be used by the caller to cancel the rendering
    std::unique_ptr<QgsFeedback> mFeedback;
    //! Zoom level at which we will be rendering
    int mTileZoom = 0;
    //! Definition of the tile matrix for our zoom level
    QgsTileMatrix mTileMatrix;
    //!< Block of tiles we will be rendering in that zoom level
    QgsTileRange mTileRange;
    //! Cached QgsFields object for each sub-layer that will be rendered
    QMap<QString, QgsFields> mPerLayerFields;
    //! Counter of total elapsed time to decode tiles (ms)
    int mTotalDecodeTime = 0;
    //! Counter of total elapsed time to render tiles (ms)
    int mTotalDrawTime = 0;
};


#endif // QGSVECTORTILELAYERRENDERER_H
