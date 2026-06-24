import { useEffect } from "react";
import { Text } from "react-native";
import AppLayout from "../components/layout/AppLayout";
import { ThingsBoardService } from "../services/thingsboard";

export default function BasicDashboardScreen() {

    useEffect(() => {

        const loadData = async () => {

            const service = new ThingsBoardService();

            const telemetry = await service.getTelemetry();

            console.log(telemetry);

        };

        loadData();

    }, []);

    return (
        <AppLayout>
            <Text>Dashboard Básico</Text>
        </AppLayout>
    );
}