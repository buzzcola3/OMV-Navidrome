<?php

namespace OMV\Engine\Module;

use OMV\Config\Database;
use OMV\Engine\Module\ServiceAbstract;
use OMV\System\Process;
use OMV\System\SystemCtl;

class Navidrome extends ServiceAbstract
{
    public function getName(): string
    {
        return "navidrome";
    }

    public function applyConfig(): void
    {
        $mkconf = file_exists("/usr/sbin/omv-mkconf") ? "omv-mkconf" : "/usr/share/openmediavault/mkconf/navidrome";
        $args = $mkconf === "omv-mkconf" ? ["navidrome"] : [];
        (new Process($mkconf, $args))->execute();

        $database = Database::getInstance();
        $object = $database->get("conf.service.navidrome");
        $enabled = (bool)$object->get("enable");
        $unit = new SystemCtl($this->getName());

        if ($enabled) {
            $unit->enable();
            // Restart quietly to tolerate the initial start right after enablement.
            $unit->restart(TRUE);
            if (!$unit->isActive()) {
                $unit->start();
            }
        } else {
            $unit->stop(TRUE);
            $unit->disable(TRUE);
        }
    }
}
