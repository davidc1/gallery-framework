import sys, os

CMD = 'echo $LD_LIBRARY_PATH | tr \':\' \'\n\' > $GALLERY_FMWK_BASEDIR/config/ldpath.txt'
print(CMD)
os.system(CMD)

fin = open("%s/config/ldpath.txt"%os.environ['GALLERY_FMWK_BASEDIR'],'r')

varout = ''

fout = open('%s/config/ldpathout.txt'%os.environ['GALLERY_FMWK_BASEDIR'],'w')

for line in fin:
    if (line.find('larlite') >= 0):
        continue
    lineout = line.replace('\n',':')
    varout += lineout

fout.write(varout)
fin.close()
fout.close()

print(varout)






