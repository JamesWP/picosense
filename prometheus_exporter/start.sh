dir=$(dirname $0)
config=$(realpath ${dir}/config.yaml)

echo using $config for exporter

podman run -it \
	-v "${config}:/config.yaml:ro" \
     	-p 9641:9641  \
	--name='mqtt2prometheus' \
	--replace \
	ghcr.io/hikhvar/mqtt2prometheus:latest
