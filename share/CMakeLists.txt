
SET(tseries_DESKTOP
  metno-tseries-${PVERSION}.desktop
)
CONFIGURE_FILE(metno-tseries.desktop.in
  ${tseries_DESKTOP}
)
INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/${tseries_DESKTOP}
  DESTINATION "share/applications"
)

INSTALL(FILES tseries.png
  DESTINATION "share/pixmaps"
  RENAME "tseries-${_PVERSION}.png"
)
