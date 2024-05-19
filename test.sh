while true; do
    read -p "Do you wish to install dependancies? [y/N]: " yn
    case $yn in
        [Yy]* ) echo "Installing"; break;;
        [Nn]* ) echo "Not installing..."; exit;;
        * ) echo "";;
    esac
done
