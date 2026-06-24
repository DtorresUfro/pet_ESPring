import { Telemetry } from "../models/Telemetry";

export class ThingsBoardService {

    async getTelemetry(): Promise<Telemetry> {

        return {
            waterLevel: 78,
            pumpStatus: true,
            petDetected: false,
            consumption: 1.2,
            lastUpdate: new Date().toISOString()
        };
    }

}