SET(LIB_REQUIRED_DEP_PACKAGES Teuchos Kokkos Sacado Phalanx Intrepid FEI ThyraCore ThyraTpetraAdapters ThyraEpetraAdapters Tpetra Epetra EpetraExt)
SET(LIB_OPTIONAL_DEP_PACKAGES STK SEACASIoss SEACASExodus Teko Stokhos MueLu)
SET(TEST_REQUIRED_DEP_PACKAGES)
SET(TEST_OPTIONAL_DEP_PACKAGES Stratimikos Piro NOX Rythmos)
SET(LIB_REQUIRED_DEP_TPLS MPI Boost)
SET(LIB_OPTIONAL_DEP_TPLS PAPI)
SET(TEST_REQUIRED_DEP_TPLS)
SET(TEST_OPTIONAL_DEP_TPLS)

TRIBITS_ALLOW_MISSING_EXTERNAL_PACKAGES(MueLu)
