#!/bin/bash


append_to_file_if_not_exist() {
  count=$(cat $1 | grep "$2" | wc -l)
  if [ 0 == $count ];then
    echo $2 >> $1
  fi
}


project_path="$(pwd)"
home_path=/home/$SUDO_USER
cd $home_path
if [ ! -d ".fastgo" ]; then
  echo create dir ~/.fastgo
  mkdir ~/.fastgo
fi
cd .fastgo

# 判断是否存在MeiliSearch服务
count=`ps -ef |grep meilisearch |grep -v "grep" |wc -l`
if [ 0 == $count ]; then
    echo install meilisearch
    #curl -L https://install.meilisearch.com | sh

    mv ./meilisearch /usr/bin/

    echo "[Unit]" > /etc/systemd/system/meilisearch.service
    echo "Description=Meilisearch" >> /etc/systemd/system/meilisearch.service
    echo "After=systemd-user-sessions.service" >> /etc/systemd/system/meilisearch.service
    echo "[Service]" >> /etc/systemd/system/meilisearch.service
    echo "Type=simple" >> /etc/systemd/system/meilisearch.service
    echo "ExecStart=/usr/bin/meilisearch --http-addr 127.0.0.1:7700" >> /etc/systemd/system/meilisearch.service
    echo "[Install]" >> /etc/systemd/system/meilisearch.service
    echo "WantedBy=default.target" >> /etc/systemd/system/meilisearch.service

    systemctl enable meilisearch
    systemctl start meilisearch
fi

# 编译项目
cd $project_path
mkdir -p build
cd build
cmake ..
make -j4
cp ./fastgo $home_path/.fastgo
cp ../fastgo.sh $home_path/.fastgo


#  安装提示
append_to_file_if_not_exist  $home_path/.bashrc  "[[ -s $home_path/.fastgo/fastgo.sh ]] && source $home_path/.fastgo/fastgo.sh"
append_to_file_if_not_exist  $home_path/.bashrc  "bind -x '\"\C-g\":go'"
echo " "
echo "reboot terminal or execute below, then enjoy fastgo"
echo "                        " "source $home_path/.bashrc"

