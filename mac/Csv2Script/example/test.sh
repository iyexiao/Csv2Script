
# 保证同目录下有csv export文件夹
#参数 1:csv目录下的csv文件名,2:项目名称GodProj（区分大小写）

FILE_NAME=$1
PROJ_NAME=$2

DIR=`pwd`

cd $DIR

./Csv2Script $FILE_NAME . $PROJ_NAME

echo "done"