DIRS =  ProjectA \
		ProjectB \
		ProjectC
		
all : lib
	@ for dir in ${DIRS}; do (cd $${dir}; ${MAKE}) ; done

clean: 
	@ for dir in ${DIRS}; do (cd $${dir}; ${MAKE} clean) ; done

lib:
	@ for dir in ${DIRS}; do (cd $${dir}; ${MAKE} lib) ; done
	
install:
	@ for dir in ${DIRS}; do (cd $${dir}; ${MAKE} install) ; done

uninstall:
	@ for dir in ${DIRS}; do (cd $${dir}; ${MAKE} uninstall) ; done
	