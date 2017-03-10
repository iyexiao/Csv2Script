
# 保证跟目录下有csv export文件夹
# 保证是在当前路径运行脚本
# 参数 1:项目名称GodProj（区分大小写）


PROJ_NAME=$1

ROOT_DIR=`pwd`

cd $ROOT_DIR

#删除旧资源
rm -rf ${ROOT_DIR}/export/*

function fncsv2TXT()
{
	for filepath in `find ${ROOT_DIR}/csv -type f -depth 1`
	do
		local filename=`echo "${filepath}" | awk -F '.' '{print $1}' `
		local filetype=`echo "${filepath}" | awk -F '.' '{print $NF}' `
		filename=`echo "${filename}" "${ROOT_DIR}/csv/" | awk '{gsub($2,"");print $NF}'` #删掉前面的的路径字符串
		if [ ${filetype} == "csv" ]
		then
			# echo "fncsv2TXT, filename=${filename}"
			./Csv2Script ${filename}.${filetype} . $PROJ_NAME
		fi
	done
}
fncsv2TXT

echo "done"
exit 0