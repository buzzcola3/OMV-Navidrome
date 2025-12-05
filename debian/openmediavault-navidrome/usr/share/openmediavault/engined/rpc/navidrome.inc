<?php


use OMV\Config\Database;
use OMV\Engine\Module\Navidrome as NavidromeModule;
use OMV\Rpc\Exception as RpcException;
use OMV\Rpc\ServiceAbstract;

class Navidrome extends ServiceAbstract
{
    public function getName(): string
    {
        return "navidrome";
    }

    public function initialize(): void
    {
        $this->registerMethod("get");
        $this->registerMethod("set");
    }

    public function get(array $params, array $context = [])
    {
        $this->validateMethodContext($context, [
            "role" => OMV_ROLE_ADMINISTRATOR,
        ]);

        $database = Database::getInstance();
        $object = $database->get("conf.service.navidrome");

        return $object->getAssoc();
    }

    public function set(array $params, array $context = [])
    {
        $this->validateMethodContext($context, [
            "role" => OMV_ROLE_ADMINISTRATOR,
        ]);
        $params = $this->normalizeParams($params);
        $this->validateMethodParams($params, "rpc.navidrome.set");
        $this->assertPathsWhenEnabled($params);

        $database = Database::getInstance();
        $object = $database->get("conf.service.navidrome");
        $object->setAssoc($params);
        $database->set($object);

        $module = new NavidromeModule();
        $module->applyConfig();

        return $this->get([], $context);
    }

    private function normalizeParams(array $params): array
    {
        if (array_key_exists("config", $params) && is_array($params["config"])) {
            $params = array_replace($params["config"], $params);
        }

        unset($params["config"]);

        return $params;
    }

    private function assertPathsWhenEnabled(array $params): void
    {
        if (empty($params["enable"])) {
            return;
        }

        foreach ([
            "music_directory" => "Music directory",
            "data_directory" => "Data directory",
        ] as $field => $label) {
            $value = (string) ($params[$field] ?? "");
            if ("" === trim($value)) {
                throw new RpcException(sprintf("%s is required when enabling Navidrome.", $label));
            }
        }
    }
}

?>
