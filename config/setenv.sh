source /grid/fermiapp/products/uboone/setup_uboone.sh
echo "Setting up uboonecode v06_50_00"
setup uboonecode v06_50_00 -q e14:prof
echo "Setting up larsoftobj v1_28_00"
setup larsoftobj v1_28_00 -q e14:prof
source /uboone/app/users/cadams/pystack2/setup.sh