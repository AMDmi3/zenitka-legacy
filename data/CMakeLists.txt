file(GLOB zenitka_BACKGROUNDS backgrounds/*.jpg)
file(GLOB zenitka_SPRITES sprites/*.png)

# targets
if(WITH_INSTALL)
	install(FILES ${zenitka_BACKGROUNDS} DESTINATION ${DATADIR}/backgrounds)
	install(FILES ${zenitka_SPRITES} DESTINATION ${DATADIR}/sprites)
endif(WITH_INSTALL)
